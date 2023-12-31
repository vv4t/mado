#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "common/log.h"
#include "game/game.h"
#include "renderer/gl.h"
#include "renderer/renderer.h"

#ifdef __EMSCRIPTEN__
  #include <emscripten.h>
#endif

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

typedef enum {
  IN_LEFT,
  IN_RIGHT,
  IN_FORWARD,
  IN_BACK,
  IN_ROT_LEFT,
  IN_ROT_RIGHT,
  IN_MOUSE_DOWN,
  MAX_INPUT
} input_t;

static struct {
  SDL_Window *window;
  SDL_GLContext gl;
  
  usercmd_t usercmd;
  game_t game;
  renderer_t renderer;

  int cursor_x;
  int cursor_y;

  float input_state[MAX_INPUT];
} nui;

bool nui_init(void);
void nui_update(void);
void nui_base_move(void);
void nui_key_event(int key, float action);
void nui_mouse_click_event(int button_type, bool state);
void nui_mouse_move_event(int x, int y);

bool sdl_poll(void);
void sdl_quit(void);
bool sdl_init(int width, int height, const char *title);

int main(int argc, char *argv[])
{
  if (!nui_init()) {
    LOG_ERROR("failed to initialise nui");
    return 1;
  }
  
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(nui_update, 0, true);
#else
  while (sdl_poll())
    nui_update();
#endif
  
  sdl_quit();
  
  return 0;
}

bool nui_init(void)
{
  if (!sdl_init(WIN_WIDTH, WIN_HEIGHT, "nui")) {
    LOG_ERROR("failed to initialise SDL");
    return false;
  }
  
  if (!gl_init()) {
    LOG_ERROR("failed to initialise GL");
    return false;
  }
  
  if (!renderer_init(&nui.renderer)) {
    LOG_ERROR("failed to initialise renderer");
    return false;
  }
  
  static tile_data_t tile_data[] = {
    { .block = { 1, 2 }, .num_block = 2, .solid = true },
    { .block = { 1, 1, 2 }, .num_block = 3, .solid = true }
  };
  
#define A (0 | TILE_DATA_EXISTS)
#define B (1 | TILE_DATA_EXISTS)
  
  static tile_t map_data[32*32] = {0};
  
  for (int i = 0; i < 3; i++) {
    map_data[rand() % (16*16)] = A;
  }
  
  static sprite_sheet_t sprite_sheet = {
    .path = "assets/texture/texture.png",
    .sprite_width = 32,
    .sprite_height = 32,
    .sheet_width = 8,
    .sheet_height = 8
  };
  
  static tile_set_t tile_set = {
    .sprite_sheet = &sprite_sheet,
    .tile_data = tile_data
  };
  
  static map_t map = {
    .data = map_data,
    .tile_set = &tile_set,
    .border_tile = A,
    .width = 16,
    .height = 16
  };
  
  game_init(&nui.game);
  game_load_map(&nui.game, &map);
  renderer_load_sheet(&nui.renderer, &sprite_sheet);
  renderer_load_map(&nui.renderer, &map);
  
  return true;
}

void nui_update(void)
{
  nui_base_move();
  game_update(&nui.game, &nui.usercmd);
  renderer_render(&nui.renderer, &nui.game);
  SDL_GL_SwapWindow(nui.window);
}

void nui_base_move(void)
{
  nui.usercmd.forward = nui.input_state[IN_FORWARD] - nui.input_state[IN_BACK];
  nui.usercmd.side = nui.input_state[IN_RIGHT] - nui.input_state[IN_LEFT];
  nui.usercmd.rot = nui.input_state[IN_ROT_LEFT] - nui.input_state[IN_ROT_RIGHT];
  nui.usercmd.aim_x = (nui.cursor_x - WIN_WIDTH / 2.0) / WIN_HEIGHT;
  nui.usercmd.aim_y = (nui.cursor_y - WIN_HEIGHT / 2.0) / WIN_HEIGHT;
  nui.usercmd.attack = nui.input_state[IN_MOUSE_DOWN];
}

void nui_key_event(int key, float action)
{
  if (key == 'a')
    nui.input_state[IN_LEFT] = action;
  if (key == 'd')
    nui.input_state[IN_RIGHT] = action;
  if (key == 'w')
    nui.input_state[IN_FORWARD] = action;
  if (key == 's')
    nui.input_state[IN_BACK] = action;
  if (key == 'q')
    nui.input_state[IN_ROT_LEFT] = action;
  if (key == 'e')
    nui.input_state[IN_ROT_RIGHT] = action;
}

void nui_mouse_click_event(int button_type, bool state)
{
  if (button_type == SDL_BUTTON_LEFT) {
    nui.input_state[IN_MOUSE_DOWN] = state;
  }
}

void nui_mouse_move_event(int x, int y)
{
  nui.cursor_x = x;
  nui.cursor_y = y;
}

bool sdl_poll(void)
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      return false;
    case SDL_KEYDOWN:
      nui_key_event(event.key.keysym.sym, 1);
      break;
    case SDL_KEYUP:
      nui_key_event(event.key.keysym.sym, 0);
      break;
    case SDL_MOUSEMOTION:
      nui_mouse_move_event(event.motion.x, event.motion.y);
      break;
    case SDL_MOUSEBUTTONDOWN:
      nui_mouse_click_event(event.button.button, true);
      break;
    case SDL_MOUSEBUTTONUP:
      nui_mouse_click_event(event.button.button, false);
      break;
    }
  }
  
  return true;
}

bool sdl_init(int width, int height, const char *title)
{
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  
  nui.window = SDL_CreateWindow(
    title,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    width,
    height,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  
  if (!nui.window) {
    LOG_ERROR("failed to create SDL window");
    return false;
  }
  
  nui.gl = SDL_GL_CreateContext(nui.window);
  
  if (!nui.gl) {
    LOG_ERROR("failed to create GL context");
    return false;
  }
  
  return true;
}

void sdl_quit(void)
{
  SDL_GL_DeleteContext(nui.gl);
  SDL_DestroyWindow(nui.window);
  SDL_Quit();
}
