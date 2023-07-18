#ifndef BULLET_H
#define BULLET_H

#include "../game.h"

void bullet_shoot(game_t *game, vec2_t pos, vec2_t uv, float angle, float live_time, c_tag_t target);

#endif
