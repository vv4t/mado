#ifndef GAME_H
#define GAME_H

#include <game/edict.h>
#include <lib/input.h>

typedef struct game_s {
  edict_t edict;
  entity_t player;
  float time;
} game_t;

void game_init(game_t *gs);
void game_update(game_t *gs, const input_t in);

#endif
