#ifndef GAME_H
#define GAME_H

#include "bullet.h"
#include "player.h"
#include "sprite.h"
#include "usercmd.h"
#include "map.h"
#include "orb.h"
#include "../common/nui_math.h"

typedef struct {
  float time;
  map_t *map;
  player_t player;
  sprite_t sprites[MAX_SPRITES];
  bullet_t bullets[MAX_BULLETS];
} game_t;

void game_init(game_t *game);
void game_load_map(game_t *game, map_t *map);
void game_update(game_t *game, const usercmd_t *usercmd);

#endif
