#ifndef GAME_H
#define GAME_H

#include "usercmd.h"
#include "../common/nui_math.h"

typedef struct {
  vec2_t pos;
  float rot;
} game_t;

void game_init(game_t *game);
void game_update(game_t *game, const usercmd_t *usercmd);

#endif
