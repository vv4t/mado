#include <client/client.h>
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
  const cl_scene_t *scene;
} cl;

static void key_press(int key, int action);
static void mouse_press(int button, int x, int y, int action);
static void mouse_move(int x, int y);

static void sdl_init();
static void sdl_deinit();

void cl_init()
{
  sdl_init();
  gfx_init();
  game_init(&cl.gs);
  
  cl.prev_time = SDL_GetTicks();
  cl.lag_time = 0;  
  
  map_t map = map_load("assets/map/1.map");
  gfx_map_load(map);
  game_map_load(&cl.gs, map);
}

int cl_update()
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
      key_press(event.key.keysym.sym, 0);
      break;
    case SDL_KEYDOWN:
      key_press(event.key.keysym.sym, 1);
      break;
    case SDL_MOUSEBUTTONUP:
      mouse_press(event.button.button, event.button.x, event.button.y, 0);
      break;
    case SDL_MOUSEBUTTONDOWN:
      mouse_press(event.button.button, event.button.x, event.button.y, 1);
      break;
    case SDL_MOUSEMOTION:
      mouse_move(event.motion.x, event.motion.y);
      break;
    }
  }
  
  if (cl.lag_time > 0) {
    cl.lag_time -= 15;
    game_update(&cl.gs, &cl.usercmd);
    cl.scene->update();
    gfx_render(&cl.gs);
    SDL_GL_SwapWindow(cl.window);
  }
  
  int now_time = SDL_GetTicks();
  cl.lag_time += now_time - cl.prev_time;
  cl.prev_time = now_time;
  
  return 1;
}

void cl_deinit()
{
  gfx_deinit();
  sdl_deinit();
}

void cl_scene_load(const cl_scene_t *scene)
{
  cl.scene = scene;
  scene->load();
}

game_t *cl_get_game()
{
  return &cl.gs;
}

void mouse_move(int x, int y)
{
  cl.usercmd.aim_x = x / (float) WIDTH - 0.5;
  cl.usercmd.aim_y = y / (float) HEIGHT - 0.5;
}

void key_press(int key, int action)
{
  gui_key_press(key, action);
  
  switch (key) {
  case 'w':
    cl.usercmd.forward = action;
    break;
  case 's':
    cl.usercmd.back = action;
    break;
  case 'a':
    cl.usercmd.left = action;
    break;
  case 'd':
    cl.usercmd.right = action;
    break;
  case 'q':
    cl.usercmd.rotate_left = action;
    break;
  case 'e':
    cl.usercmd.rotate_right = action;
    break;
  }
}

void mouse_press(int button, int x, int y, int action)
{
  if (button == 1) {
    gui_mouse_press(x / (float) HEIGHT, y / (float) HEIGHT, action);
    cl.usercmd.attack = action;
  }
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
  
  cl.window = SDL_CreateWindow(
    "mado",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WIDTH,
    HEIGHT,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
  );
  
  cl.gl = SDL_GL_CreateContext(cl.window);
  
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
  SDL_GL_DeleteContext(cl.gl);
  SDL_DestroyWindow(cl.window);
  SDL_Quit();
}
