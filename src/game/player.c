#include "player.h"
#include "bullet.h"
#include "game.h"

vec2_t player_cmd_move(player_t *player, float delta_time, const usercmd_t *usercmd);
void player_collide_move(player_t *player, const map_t *map, vec2_t move_dir);
void player_sprite_anim(player_t *player, float time, const usercmd_t *usercmd);

void player_init(player_t *player, sprite_t *sprite)
{
  sprite->pos = vec2_init(0.0, 0.0);
  sprite->uv = vec2_init(0, 5);
  sprite->show = true;
  sprite->stand = true;
  
  player->pos = vec2_init(2.0, 2.0);
  player->sprite = sprite;
  
  player->anim_move_right   = (anim_t) { .start_uv = {0,5}, .frame_count = 2, .frame_time = 0.2 };
  player->anim_move_left    = (anim_t) { .start_uv = {2,5}, .frame_count = 2, .frame_time = 0.2 };
  player->anim_move_forward = (anim_t) { .start_uv = {2,6}, .frame_count = 2, .frame_time = 0.2 };
  player->anim_move_back    = (anim_t) { .start_uv = {0,6}, .frame_count = 2, .frame_time = 0.2 };
  
  player->speed = 7.5;

  player->shoot_cooldown = 0;
  player->max_shoot_cooldown = 5;
  player->shoot_cooldown_decay = 5;
}

void player_move(
  player_t *player, 
  const map_t *map, 
  float time, 
  float delta_time,
  const usercmd_t *usercmd
) {
  player->rot += usercmd->rot * delta_time * 5;

  vec2_t move_dir = player_cmd_move(player, delta_time, usercmd);
  player_collide_move(player, map, move_dir);
  player_sprite_anim(player, time, usercmd);
}

void player_sprite_anim(player_t *player, float time, const usercmd_t *usercmd)
{
  if (usercmd->side < 0) {
    play_anim_play(&player->play_anim, &player->anim_move_left, time);
  } else if (usercmd->side > 0) {
    play_anim_play(&player->play_anim, &player->anim_move_right, time);
  } else if (usercmd->forward > 0) {
    play_anim_play(&player->play_anim, &player->anim_move_forward, time);
  } else if (usercmd->forward < 0) {
    play_anim_play(&player->play_anim, &player->anim_move_back, time);
  } else {
    
  }
  
  sprite_play_anim(player->sprite, &player->play_anim);
  player->sprite->pos = player->pos;
}

vec2_t player_cmd_move(player_t *player, float delta_time, const usercmd_t *usercmd)
{
  vec2_t cmd_dir = vec2_init(usercmd->side, usercmd->forward);
  vec2_t wish_dir = vec2_normalize(cmd_dir);
  vec2_t rot_dir = vec2_rotate(wish_dir, player->rot);
  vec2_t move_dir = vec2_mulf(rot_dir, player->speed * delta_time);
  
  return move_dir;
}

void player_collide_move(player_t *player, const map_t *map, vec2_t move_dir)
{
  vec2_t new_pos = vec2_add(player->pos, move_dir);
  vec2_t new_pos_x = vec2_add(player->pos, vec2_init(move_dir.x, 0.0));
  vec2_t new_pos_y = vec2_add(player->pos, vec2_init(0.0, move_dir.y));
  vec2_t box = vec2_init(0.2, 0.2);
  
  if (map_collide(map, new_pos.x, new_pos.y, box)) {
    if (!map_collide(map, player->pos.x, new_pos.y, box)) {
      player->pos.y = new_pos.y;
    } else if (!map_collide(map, new_pos.x, player->pos.y, box)) {
      player->pos.x = new_pos.x;
    } 
  } else {
    player->pos = new_pos;
  }
}

void player_shoot(
  player_t *player, 
  bullet_t bullets[MAX_BULLETS], 
  sprite_t sprites[MAX_SPRITES],
  float delta_time,
  const usercmd_t *usercmd
) {
  if (player->shoot_cooldown > 0) {
    player->shoot_cooldown -= player->shoot_cooldown_decay * (delta_time * 10);
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
      10, 50, 5
    );
  }
}
