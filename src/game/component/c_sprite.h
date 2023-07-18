#ifndef C_SPRITE_H
#define C_SPRITE_H

#include "../sprite.h"

typedef struct {
  vec2_t uv;
  float rotation;
  bool orient;
  bool stand;
} c_sprite_t;

inline void c_sprite_init(c_sprite_t *c_sprite, vec2_t uv, bool orient, bool stand, float rotation)
{
  c_sprite->uv = uv;
  c_sprite->stand = orient;
  c_sprite->orient = stand;
  c_sprite->rotation = rotation;
}

#endif
