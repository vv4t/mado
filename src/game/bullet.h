#ifndef BULLET_H
#define BULLET_H

#include "sprite.h"
#include "../common/nui_math.h"

#define MAX_BULLETS 20

typedef struct {
  bool used;
  vec2_t pos;
  float rot;
  sprite_t *sprite;
} bullet_t;

bullet_t *bullet_new(bullet_t bullets[MAX_BULLETS]);

#endif