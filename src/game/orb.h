#ifndef ORB_H
#define ORB_H

#include "sprite.h"

typedef struct {
  vec2_t pos;
  sprite_t *sprite;
  anim_t idle_anim;
  play_anim_t play_anim;
} orb_t;

void orb_init(orb_t *orb, sprite_t *sprite);
void orb_update(orb_t *orb, float time);

#endif
