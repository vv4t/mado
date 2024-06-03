#include <client/window.h>
#include <lib/log.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>

struct {
  SDL_Window *window;
  SDL_GLContext *gl;
  int width;
  int height;
} window;

static int window_poll(input_t in);

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

void window_init(int width, int height, const char *title)
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
    title,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    width,
    height,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
  );
  
  window.gl = SDL_GL_CreateContext(window.window);
  window.width = width;
  window.height = height;
  
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

int window_loop(input_t in)
{
  if (!window_poll(in)) {
    return 0;
  }
  
  SDL_GL_SwapWindow(window.window);
  
  return 1;
}

int window_poll(input_t in)
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      return 0;
    case SDL_KEYUP:
      input_key_up(in, event.key.keysym.sym);
      break;
    case SDL_KEYDOWN:
      input_key_down(in, event.key.keysym.sym);
    case SDL_MOUSEBUTTONDOWN:
      input_mouse_down(in, event.button.button);
      break;
    case SDL_MOUSEBUTTONUP:
      input_mouse_up(in, event.button.button);
      break;
    case SDL_MOUSEMOTION:
      input_mouse_move(in, event.motion.x / (float) window.width, event.motion.y / (float) window.height);
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

int window_get_time() {
  return SDL_GetTicks();
}
