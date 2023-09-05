#ifndef C_SPRITE_H
#define C_SPRITE_H

#include "component.h"
#include "../sprite.h"

typedef struct {
  vec2_t uv;
  vec2_t spr_size;
  vec2_t size;
  float rotation;
  bool orient;
  bool stand;
} c_sprite_t;

inline component_t c_sprite_init(c_sprite_t *c_sprite, vec2_t uv)
{
  c_sprite->uv = uv;
  c_sprite->spr_size = vec2_init(1.0, 1.0);
  c_sprite->size = vec2_init(1.0, 1.0);
  c_sprite->stand = true;
  c_sprite->orient = true;
  c_sprite->rotation = 0.0;
  return COMPONENT_SPRITE;
}

#endif
