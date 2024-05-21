#ifndef GAME_H
#define GAME_H

#include <game/edict.h>
#include <lib/input.h>
#include <lib/map.h>

typedef struct game_s {
  map_t map;
  edict_t edict;
  entity_t player;
  
  vector view_position;
  vector view_rotation;
  
  float time;
} game_t;

void game_init(game_t *gs);
void game_load_map(game_t *gs, map_t map);
void game_update(game_t *gs, const input_t in);

#endif
