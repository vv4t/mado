#ifndef TEXTURE_H
#define TEXTURE_H

#include "gl.h"
#include <SDL2/SDL_image.h>

bool texture_load(GLuint *texture, const char *path);

#endif
