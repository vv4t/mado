#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"
#include "bullet.h"
#include "map.h"
#include "usercmd.h"
#include "game.h"
#include "../common/nui_math.h"

typedef struct {
  vec2_t pos;
  float rot;
  sprite_t *sprite;
} player_t;

void player_init(player_t *player, sprite_t *sprite);
void player_move(player_t *player, map_t *map, float time, const usercmd_t *usercmd);
void player_shoot(
  player_t *player, 
  bullet_t bullets[MAX_BULLETS], 
  sprite_t sprites[MAX_SPRITES],
  const usercmd_t *usercmd
);

#endif
