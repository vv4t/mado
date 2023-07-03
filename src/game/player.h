#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"
#include "map.h"
#include "usercmd.h"
#include "../common/nui_math.h"

typedef struct {
  vec2_t pos;
  float rot;
  sprite_t *sprite;
} player_t;

void player_init(player_t *player, sprite_t *sprite);
void player_move(player_t *player, const map_t *map, float time, const usercmd_t *usercmd);

#endif
