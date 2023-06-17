#ifndef GL_H
#define GL_H

#ifdef __EMSCRIPTEN__
  #include <SDL_opengles2.h>
  #include <GLES3/gl3.h>
  #include <GLES3/gl2ext.h>
#else
  #include <GL/glew.h>
#endif

#include <stdbool.h>
#include <SDL2/SDL_opengl.h>

bool gl_init();

#endif
