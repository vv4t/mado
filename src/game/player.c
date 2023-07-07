#include "player.h"

#include "system.h"

static void player_move(player_t *player, edict_t *edict, const usercmd_t *usercmd);
static void player_animate(player_t *player, edict_t *edict, const usercmd_t *usercmd);
static void player_aim(player_t *player, edict_t *edict, const usercmd_t *usercmd);
static void player_attack(entity_t entity, edict_t *edict);

static animation_t player_anim_move_right   = (animation_t) { .uv = {0,5}, .frame_count = 2, .frame_time = 0.2 };
static animation_t player_anim_move_left    = (animation_t) { .uv = {2,5}, .frame_count = 2, .frame_time = 0.2 };
static animation_t player_anim_move_forward = (animation_t) { .uv = {2,6}, .frame_count = 2, .frame_time = 0.2 };
static animation_t player_anim_move_back    = (animation_t) { .uv = {0,6}, .frame_count = 2, .frame_time = 0.2 };

void player_init(player_t *player, edict_t *edict)
{
  player->entity = edict_spawn(edict);
  edict->field[player->entity] |= COMPONENT_TRANSFORM;
  edict->field[player->entity] |= COMPONENT_MOTION;
  edict->field[player->entity] |= COMPONENT_SPRITE;
  edict->field[player->entity] |= COMPONENT_ANIMATOR;
  edict->field[player->entity] |= COMPONENT_ACTION;
  
  edict->transform[player->entity].position = vec2_init(2.0, 2.0);
  
  edict->sprite[player->entity].stand = true;
  edict->sprite[player->entity].orient = true;
  edict->sprite[player->entity].rotation = 0.0;
  play_animation(&edict->animator[player->entity], &player_anim_move_left);
  
  edict->action[player->entity].attack[0].xaction = player_attack;
  edict->action[player->entity].attack[0].time = 0.0;
  edict->action[player->entity].attack[0].cooldown = 0.25;
  edict->action[player->entity].attack[0].active = false;
  edict->action[player->entity].attack[0].count = 0;
  edict->action[player->entity].num_attack = 1;
}

void player_update(player_t *player, edict_t *edict, const usercmd_t *usercmd)
{
  player_aim(player, edict, usercmd);
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
    play_animation(&edict->animator[player->entity], &player_anim_move_left);
  } else if (usercmd->side > 0) {
    play_animation(&edict->animator[player->entity], &player_anim_move_right);
  } else if (usercmd->forward > 0) {
    play_animation(&edict->animator[player->entity], &player_anim_move_forward);
  } else if (usercmd->forward < 0) {
    play_animation(&edict->animator[player->entity], &player_anim_move_back);
  }
}

void player_aim(player_t *player, edict_t *edict, const usercmd_t *usercmd) {
  float player_angle = edict->transform[player->entity].rotation; 
  float shoot_angle = player_angle - atan2(usercmd->aim_y, usercmd->aim_x);
  
  edict->action[player->entity].angle = shoot_angle;
  edict->action[player->entity].attack[0].active = usercmd->attack;
}

void player_attack(entity_t entity, edict_t *edict)
{
  shoot_bullet(edict, edict->transform[entity].position, edict->action[entity].angle, 1.0);
}
