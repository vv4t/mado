#include <client/window.h>
#include <client/input.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <lib/log.h>

#define WIDTH 1280
#define HEIGHT 960

struct {
  SDL_Window *window;
  SDL_GLContext *gl;
} window;

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
    LOG_ERROR(
      "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
      (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
      type,
      severity,
      message
    );
  }
}

void window_init()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    LOG_ERROR("SDL_Error: %s", SDL_GetError());
  }
  
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  window.window = SDL_CreateWindow(
    "mado",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WIDTH,
    HEIGHT,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
  );
  
  window.gl = SDL_GL_CreateContext(window.window);

#ifndef __EMSCRIPTEN__ 
  glewExperimental = 1;
  GLenum status = glewInit();
  if (status != GLEW_OK) {
    LOG_ERROR("GLEW_Error: %s", glewGetErrorString(status));
  }
  
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
#endif
}

int window_poll()
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      return 0;
    case SDL_KEYUP:
      input_key_press(event.key.keysym.sym, 0);
      break;
    case SDL_KEYDOWN:
      input_key_press(event.key.keysym.sym, 1);
      break;
    case SDL_MOUSEBUTTONUP:
      input_mouse_press(event.button.button, 0);
      break;
    case SDL_MOUSEBUTTONDOWN:
      input_mouse_press(event.button.button, 1);
      break;
    case SDL_MOUSEMOTION:
      input_mouse_move(event.motion.x / (float) WIDTH, event.motion.y / (float) HEIGHT);
      break;
    default:
      break;
    }
  }
  
  return 1;
}

void window_deinit()
{
  SDL_GL_DeleteContext(window.gl);
  SDL_DestroyWindow(window.window);
  SDL_Quit();
}

int window_get_time()
{
  return SDL_GetTicks();
}

void window_swap()
{
  SDL_GL_SwapWindow(window.window);
}
