#ifndef BULLET_H
#define BULLET_H

#include "../game.h"

typedef struct {
  vec2_t  uv;
  c_tag_t target;
  float   live_time;
  float   speed;
  int     damage;
} shooter_t;

void bullet_shoot(game_t *game, const shooter_t *shooter, vec2_t pos, float angle);

#endif
