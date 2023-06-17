#ifndef SHADER_H
#define SHADER_H

#include "gl.h"

bool shader_load(
  GLuint      *program,
  const char  *define,
  const char  *src_vertex,
  const char  *src_fragment);

#endif
