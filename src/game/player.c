#include "player.h"

#include "system.h"

void player_move(player_t *player, edict_t *edict, const usercmd_t *usercmd);
void player_shoot(player_t *player, edict_t *edict, const usercmd_t *usercmd);
void player_animate(player_t *player, edict_t *edict, const usercmd_t *usercmd);

void player_init(player_t *player, edict_t *edict)
{
  player->entity = edict_spawn(edict);
  edict->field[player->entity] |= COMPONENT_TRANSFORM;
  edict->field[player->entity] |= COMPONENT_MOTION;
  edict->field[player->entity] |= COMPONENT_SPRITE;
  edict->field[player->entity] |= COMPONENT_ANIMATOR;
  
  edict->sprite[player->entity].stand = true;
  edict->sprite[player->entity].orient = true;
  edict->sprite[player->entity].rotation = 0.0;
  play_animation(&edict->animator[player->entity], &player->anim_move_left);
  
  edict->transform[player->entity].position = vec2_init(2.0, 2.0);
  
  player->anim_move_right   = (animation_t) { .uv = {0,5}, .frame_count = 2, .frame_time = 0.2 };
  player->anim_move_left    = (animation_t) { .uv = {2,5}, .frame_count = 2, .frame_time = 0.2 };
  player->anim_move_forward = (animation_t) { .uv = {2,6}, .frame_count = 2, .frame_time = 0.2 };
  player->anim_move_back    = (animation_t) { .uv = {0,6}, .frame_count = 2, .frame_time = 0.2 };

  player->shoot_cooldown = 0;
  player->max_shoot_cooldown = 5;
  player->shoot_cooldown_decay = 5;
}

void player_update(player_t *player, edict_t *edict, const usercmd_t *usercmd)
{
  player_shoot(player, edict, usercmd);
  player_move(player, edict, usercmd);
  player_animate(player, edict, usercmd);
}

void player_move(player_t *player, edict_t *edict, const usercmd_t *usercmd)
{
  edict->transform[player->entity].rotation += usercmd->rot * 3.0 * DELTA_TIME;
  
  vec2_t cmd_dir = vec2_init(usercmd->side, usercmd->forward);
  vec2_t wish_dir = vec2_normalize(cmd_dir);
  vec2_t rot_dir = vec2_rotate(wish_dir, edict->transform[player->entity].rotation);
  vec2_t move_dir = vec2_mulf(rot_dir, 3.0);
  
  edict->motion[player->entity].velocity = move_dir;
}

void player_animate(player_t *player, edict_t *edict, const usercmd_t *usercmd)
{
  if (usercmd->side < 0) {
    play_animation(&edict->animator[player->entity], &player->anim_move_left);
  } else if (usercmd->side > 0) {
    play_animation(&edict->animator[player->entity], &player->anim_move_right);
  } else if (usercmd->forward > 0) {
    play_animation(&edict->animator[player->entity], &player->anim_move_forward);
  } else if (usercmd->forward < 0) {
    play_animation(&edict->animator[player->entity], &player->anim_move_back);
  }
}

void player_shoot(player_t *player, edict_t *edict, const usercmd_t *usercmd) {
  if (player->shoot_cooldown > 0) {
    player->shoot_cooldown -= player->shoot_cooldown_decay * DELTA_TIME;
  }

  if (!usercmd->mouse_down) {
    return;
  }

  if (player->shoot_cooldown <= 0) {
    player->shoot_cooldown = player->max_shoot_cooldown;
    float shoot_angle = edict->transform[player->entity].rotation - atan2(usercmd->relative_cursor_y, usercmd->relative_cursor_x);
    
    shoot_bullet(edict, edict->transform[player->entity].position, shoot_angle, 1.0);
  }
}
