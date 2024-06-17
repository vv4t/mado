#include <game/game.h>
#include <lib/map.h>
#include <lib/log.h>
#include <gfx/gfx.h>
#include <gfx/gui.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH 1280
#define HEIGHT 960

struct {
  SDL_Window *window;
  SDL_GLContext *gl;
  
  int prev_time;
  int lag_time;
  
  usercmd_t usercmd;
  game_t gs;
  bool focus_game;
} client;

static void init();
static void deinit();
static void update();
static int poll();

static void key_press(const SDL_Event *event, int action);
static void mouse_press(const SDL_Event *event, int action);
static void mouse_move(const SDL_Event *event);

static void cl_hud_init();

static void sdl_init();
static void sdl_deinit();

int main(int argc, char *argv[])
{
  init();
  while (poll()) update();
  deinit();
  return 0;
}

void inputbox_invoke(gui_node_t node, gui_event_t event)
{
  printf("%s\n", gui_inputbox_get_value(node));
}

void init()
{
  sdl_init();
  gfx_init();
  cl_hud_init();
  game_init(&client.gs);
  
  map_t map = map_load("assets/map/1.map");
  
  gfx_load_map(map);
  game_load_map(&client.gs, map);
  
  client.prev_time = SDL_GetTicks();
  client.lag_time = 0;
}

void update()
{
  if (client.lag_time > 0) {
    client.lag_time -= 15;
    game_update(&client.gs, &client.usercmd);
    gfx_render(&client.gs);
    SDL_GL_SwapWindow(client.window);
  }
  
  int now_time = SDL_GetTicks();
  client.lag_time += now_time - client.prev_time;
  client.prev_time = now_time;
}

void cl_hud_init()
{
  gui_node_t inputbox = gui_create_inputbox(64);
    gui_inputbox_resize(inputbox, 16.0 / 980.0);
    gui_node_bind(inputbox, inputbox_invoke);
  gui_push(inputbox);
  gui_focus(inputbox);
}

int poll()
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      return 0;
    case SDL_TEXTINPUT:
      gui_text_input(event.text.text);
      break;
    case SDL_KEYUP:
      key_press(&event, 0);
      break;
    case SDL_KEYDOWN:
      key_press(&event, 1);
      break;
    case SDL_MOUSEBUTTONUP:
      mouse_press(&event, 0);
      break;
    case SDL_MOUSEBUTTONDOWN:
      mouse_press(&event, 1);
      break;
    case SDL_MOUSEMOTION:
      mouse_move(&event);
      break;
    }
  }
  
  return 1;
}

void mouse_move(const SDL_Event *event)
{
  client.usercmd.aim_x = event->motion.x / (float) WIDTH - 0.5;
  client.usercmd.aim_y = event->motion.y / (float) HEIGHT - 0.5;
}

void key_press(const SDL_Event *event, int action)
{
  if (action == 1) {
    gui_key_press(event->key.keysym.sym);
  }
  
  switch (event->key.keysym.sym) {
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

void mouse_press(const SDL_Event *event, int action)
{
  if (event->button.button == 1 && action == 1) {
    gui_click(event->button.x / (float) HEIGHT, event->button.y / (float) HEIGHT);
  }
  
  switch (event->button.button) {
  case 1:
    client.usercmd.attack = action;
    break;
  }
}

void deinit()
{
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
