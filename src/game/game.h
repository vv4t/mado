#ifndef GAME_H
#define GAME_H

#include "edict.h"
#include "system.h"
#include "player.h"
#include "usercmd.h"
#include "map.h"
#include "../common/nui_math.h"
#include <time.h>

typedef struct {
  long int time;
  float delta_time;
  float animation_time;
  map_t *map;
  edict_t edict;
  player_t player;
} game_t;

void game_init(game_t *game);
void game_load_map(game_t *game, map_t *map);
void game_update(game_t *game, const usercmd_t *usercmd);

#endif
