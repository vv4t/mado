#ifndef C_SPRITE_H
#define C_SPRITE_H

#include "../sprite.h"

typedef struct {
  vec2_t uv;
  float rotation;
  bool orient;
  bool stand;
} c_sprite_t;

#endif
