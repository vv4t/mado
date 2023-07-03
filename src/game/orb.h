#ifndef ORB_H
#define ORB_H

#include "sprite.h"

typedef struct {
  vec2_t pos;
  sprite_t *sprite;
} orb_t;

void orb_init(orb_t *orb, sprite_t *sprite);
void orb_update(orb_t *orb, float time);

#endif
