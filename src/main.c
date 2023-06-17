#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "common/log.h"
#include "renderer/gl.h"
#include "renderer/renderer.h"

#ifdef __EMSCRIPTEN__
  #include <emscripten.h>
#endif

static struct {
  SDL_Window *window;
  SDL_GLContext gl;
  
  renderer_t renderer;
} nui;

bool nui_init(void);
void nui_update(void);

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
  if (!sdl_init(1280, 720, "nui")) {
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
  
  return true;
}

void nui_update(void)
{
  renderer_render(&nui.renderer);
  SDL_GL_SwapWindow(nui.window);
}

bool sdl_poll(void)
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      return false;
    case SDL_KEYUP:
      break;
    case SDL_KEYDOWN:
      break;
    case SDL_MOUSEMOTION:
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
