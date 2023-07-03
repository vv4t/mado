#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "sprite.h"
#include "usercmd.h"
#include "map.h"
#include "orb.h"
#include "../common/nui_math.h"

#define MAX_SPRITES 8
#define MAX_ORBS 8

typedef struct {
  float time;
  map_t *map;
  player_t player;
  orb_t orbs[MAX_ORBS];
  sprite_t sprites[MAX_SPRITES];
} game_t;

void game_init(game_t *game);
void game_load_map(game_t *game, map_t *map);
void game_update(game_t *game, const usercmd_t *usercmd);

#endif
