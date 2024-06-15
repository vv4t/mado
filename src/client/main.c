#include <game/game.h>
#include <lib/map.h>
#include <lib/log.h>
#include <gfx/gfx.h>
#include <gfx/gui.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>

#define WIDTH 1280
#define HEIGHT 960

struct {
  SDL_Window *window;
  SDL_GLContext *gl;
  usercmd_t usercmd;
  int prev_time;
  int lag_time;
  game_t gs;
} client;

static void init();
static void deinit();
static void update();
static void key_press(int key, int action);
static void mouse_press(int button, int action);
static int poll();

static void sdl_init();
static void sdl_deinit();

int main(int argc, char *argv[])
{
  init();
  while (poll()) update();
  deinit();
  return 0;
}

void init()
{
  sdl_init();
  gfx_init();
  game_init(&client.gs);
  
  gui_node_t box = gui_create_box();
    gui_node_move(box, 0.01565, 0.01565);
    gui_node_color(box, vec4(0.3, 0.3, 0.3, 0.5));
    gui_box_resize(box, 0.8, 0.4);
  gui_node_t text = gui_create_text(12, 4);
    gui_node_color(text, vec4(1, 1, 1, 1));
    gui_node_move(text, 0.01 * 0.5, 0.01);
    gui_text_resize(text, 0.125 / 2 / 2);
    gui_text_printf(text, "there is alot of text that you can put");
  gui_node_attach(box, text);
  gui_push(box);
  
  map_t map = map_load("assets/map/1.map");
  
  gfx_load_map(map);
  game_load_map(&client.gs, map);
  
  client.prev_time = SDL_GetTicks();
  client.lag_time = 0;
}

void update()
{
  if (client.lag_time > 15) {
    client.lag_time -= 15;
    game_update(&client.gs, &client.usercmd);
    gfx_render(&client.gs);
    SDL_GL_SwapWindow(client.window);
  }
  
  int now_time = SDL_GetTicks();
  client.lag_time += now_time - client.prev_time;
  client.prev_time = now_time;
}

int poll()
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      return 0;
    case SDL_KEYUP:
      key_press(event.key.keysym.sym, 0);
      break;
    case SDL_KEYDOWN:
      key_press(event.key.keysym.sym, 1);
    case SDL_MOUSEBUTTONUP:
      mouse_press(event.button.button, 0);
      break;
    case SDL_MOUSEBUTTONDOWN:
      mouse_press(event.button.button, 1);
      break;
    case SDL_MOUSEMOTION:
      // input_mouse_move(in, event.motion.x / (float) WIDTH, event.motion.y / (float) HEIGHT);
      break;
    }
  }
  
  return 1;
}

void key_press(int key, int action)
{
  switch (key) {
  case 'w':
    client.usercmd.forward = action;
    break;
  case 's':
    client.usercmd.back = action;
    break;
  case 'a':
    client.usercmd.left = action;
    break;
  case 'd':
    client.usercmd.right = action;
    break;
  case 'q':
    client.usercmd.rotate_left = action;
    break;
  case 'e':
    client.usercmd.rotate_right = action;
    break;
  } 
}

void mouse_press(int button, int action)
{
  switch (button) {
  case 1:
    client.usercmd.attack = action;
    break;
  }
}

void deinit()
{
  // game_deinit(&client.gs);
  gfx_deinit();
  sdl_deinit();
}

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  if (type == GL_DEBUG_TYPE_ERROR) {
    fprintf(
      stderr,
      "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
      (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
      type,
      severity,
      message
    );
  }
}

void sdl_init()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    LOG_ERROR("SDL_Error: %s", SDL_GetError());
  }
  
  client.window = SDL_CreateWindow(
    "mado",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WIDTH,
    HEIGHT,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
  );
  
  client.gl = SDL_GL_CreateContext(client.window);
  
  glewExperimental = 1;
  GLenum status = glewInit();
  if (status != GLEW_OK) {
    LOG_ERROR("GLEW_Error: %s", glewGetErrorString(status));
  }
  
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
}

void sdl_deinit()
{
  SDL_GL_DeleteContext(client.gl);
  SDL_DestroyWindow(client.window);
  SDL_Quit();
}
