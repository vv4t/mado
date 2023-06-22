#ifndef SPRITE_H
#define SPRITE_H

#include "../common/nui_math.h"
#include <stdbool.h>

typedef struct {
  vec2_t pos;
  vec2_t uv;
  bool show;
  bool stand;
} sprite_t;

typedef struct {
  const char *path;
  int sprite_width;
  int sprite_height;
  int sheet_width;
  int sheet_height;
} sprite_sheet_t;

#endif
