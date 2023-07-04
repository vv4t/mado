#include "player.h"
#include "bullet.h"
#include "game.h"

void player_init(player_t *player, sprite_t *sprite)
{
  player->pos = vec2_init(2.0, 2.0);
  player->sprite = sprite;
  player->shoot_cooldown = 0;

  player->max_shoot_cooldown = 100;
  player->shoot_cooldown_decay = 5;
}

void player_move(player_t *player, map_t *map, float time, const usercmd_t *usercmd)
{
  player->rot += usercmd->rot * 0.05;
  
  vec2_t wish_dir = vec2_init(usercmd->side, usercmd->forward);
  vec2_t move_dir = vec2_mulf(vec2_rotate(wish_dir, player->rot), 0.05);
  
  vec2_t new_pos = vec2_add(player->pos, move_dir);
  vec2_t new_pos_x = vec2_add(player->pos, vec2_init(move_dir.x, 0.0));
  vec2_t new_pos_y = vec2_add(player->pos, vec2_init(0.0, move_dir.y));
  vec2_t box = vec2_init(0.2, 0.2);
  
  if (map_collide(map, new_pos, box)) {
    if (!map_collide(map, new_pos_y, box)) {
      player->pos = new_pos_y;
    } else if (!map_collide(map, new_pos_x, box)) {
      player->pos = new_pos_x;
    } 
  } else {
    player->pos = new_pos;
  }
  
  if (fabs(move_dir.x) + fabs(move_dir.y) > 0.04)
    player->sprite->uv.x = floor(cos(time * 8) * cos(time * 8) * 2);
  
  player->sprite->pos = player->pos;
}

void player_shoot(
  player_t *player, 
  bullet_t bullets[MAX_BULLETS], 
  sprite_t sprites[MAX_SPRITES], 
  const usercmd_t *usercmd
) {
  if (player->shoot_cooldown > 0) {
    player->shoot_cooldown -= player->shoot_cooldown_decay;
  }

  if (!usercmd->mouse_down) {
    return;
  }

  if (player->shoot_cooldown <= 0) {
    player->shoot_cooldown = player->max_shoot_cooldown;

    float shoot_angle = -atan2(
      usercmd->relative_cursor_y, 
      usercmd->relative_cursor_x
    ) + player->rot;

    bullet_t *new_bullet = bullet_new(
      bullets, sprites,
      vec2_init(0,5),
      player->pos, shoot_angle, 
      100, 5
    );
  }
}