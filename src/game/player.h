#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"
#include "bullet.h"
#include "map.h"
#include "usercmd.h"
#include "../common/nui_math.h"

typedef struct {
  sprite_t *sprite;
  vec2_t pos;
  float rot;
  
  play_anim_t play_anim;
  anim_t anim_move_left;
  anim_t anim_move_right;
  anim_t anim_move_forward;
  anim_t anim_move_back;
  
  float speed;

  float shoot_cooldown;
  float max_shoot_cooldown;
  float shoot_cooldown_decay;
} player_t;

void player_init(player_t *player, sprite_t *sprite);
void player_move(
  player_t *player, 
  const map_t *map, 
  float time, 
  float delta_time,
  const usercmd_t *usercmd
);
void player_shoot(
  player_t *player, 
  bullet_t bullets[MAX_BULLETS], 
  sprite_t sprites[MAX_SPRITES],
  float delta_time,
  const usercmd_t *usercmd
);

#endif
