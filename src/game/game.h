#ifndef GAME_H
#define GAME_H

#include "edict.h"
#include "usercmd.h"
#include "map.h"
#include "component/cdict.h"

#define DELTA_TIME 0.015

typedef struct game_s {
  map_t *map;
  edict_t edict;
  cdict_t cdict;
  entity_t player;
  usercmd_t usercmd;
} game_t;

void game_kill(game_t *game, entity_t entity);

void game_init(game_t *game);
void game_load_map(game_t *game, map_t *map);
void game_update(game_t *game, const usercmd_t *usercmd);

#endif
