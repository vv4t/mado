#ifndef GAME_H
#define GAME_H

#include "usercmd.h"
#include "../common/nui_math.h"
#include <stdbool.h>

#define MAX_SPRITES 8

typedef struct {
  vec2_t pos;
  bool show;
  bool stand;
} sprite_t;

typedef struct {
  vec2_t pos;
  float rot;
  sprite_t *sprite;
} player_t;

typedef struct {
  player_t player;
  sprite_t sprites[MAX_SPRITES];
} game_t;

void game_init(game_t *game);
void game_update(game_t *game, const usercmd_t *usercmd);

#endif
