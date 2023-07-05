#ifndef BULLET_H
#define BULLET_H

#include "sprite.h"
#include "../common/nui_math.h"

#define MAX_BULLETS 100

typedef struct {
  bool used;

  sprite_t *sprite;
  vec2_t pos;
  float rot;

  float speed;
  float lifetime;
  float decay_rate;
} bullet_t;

bullet_t *bullet_new(
    bullet_t bullets[MAX_BULLETS],
    sprite_t sprites[MAX_SPRITES],
    vec2_t sprite_uv,
    vec2_t pos,
    float rot,
    float speed,
    float lifetime,
    float decay_rate
);
bullet_t *bullet_alloc(bullet_t bullets[MAX_BULLETS]);
void bullet_kill(bullet_t *bullet);
void bullet_update(bullet_t *bullet, float delta_time);

#endif