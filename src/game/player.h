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
  play_anim_t play_anim;
  anim_t anim_move_left;
  anim_t anim_move_right;
  anim_t anim_move_forward;
  anim_t anim_move_back;
} player_t;

void player_init(player_t *player, sprite_t *sprite);
void player_move(player_t *player, const map_t *map, float time, const usercmd_t *usercmd);

#endif
