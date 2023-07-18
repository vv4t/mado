#include "player.h"

#include "../system/system.h"
#include "bullet.h"

static void player_move(entity_t entity, game_t *game, const usercmd_t *usercmd);
static void player_animate(entity_t entity, game_t *game, const usercmd_t *usercmd);
static void player_aim(entity_t entity, game_t *game, const usercmd_t *usercmd);
static void player_attack(entity_t entity, game_t *game);

static animation_t player_anim_move_right   = (animation_t) { .uv = {0,5}, .frame_count = 2, .frame_time = 0.2 };
static animation_t player_anim_move_left    = (animation_t) { .uv = {2,5}, .frame_count = 2, .frame_time = 0.2 };
static animation_t player_anim_move_forward = (animation_t) { .uv = {2,6}, .frame_count = 2, .frame_time = 0.2 };
static animation_t player_anim_move_back    = (animation_t) { .uv = {0,6}, .frame_count = 2, .frame_time = 0.2 };

void player_init(entity_t entity, game_t *game)
{
  entity = edict_spawn(&game->edict);
  game->edict.field[entity] |= COMPONENT_TRANSFORM;
  game->edict.field[entity] |= COMPONENT_MOTION;
  game->edict.field[entity] |= COMPONENT_SPRITE;
  game->edict.field[entity] |= COMPONENT_ANIMATOR;
  game->edict.field[entity] |= COMPONENT_ACTOR;
  game->edict.field[entity] |= COMPONENT_BOX;
  game->edict.field[entity] |= COMPONENT_TAG;
  
  game->cdict.tag[entity] |= TAG_PLAYER;
  
  game->cdict.transform[entity].position = vec2_init(2.0, 2.0);
  
  game->cdict.sprite[entity].stand = true;
  game->cdict.sprite[entity].orient = true;
  game->cdict.sprite[entity].rotation = 0.0;
  c_animator_play(&game->cdict.animator[entity], &player_anim_move_left);
  
  c_actor_set_act(&game->cdict.actor[entity], 0, player_attack, 0.25);
  
  game->cdict.box[entity].min = vec2_init(-0.2, -0.2);
  game->cdict.box[entity].max = vec2_init(+0.2, +0.2);
}

void player_update(entity_t entity, game_t *game, const usercmd_t *usercmd)
{
  player_aim(entity, game, usercmd);
  player_move(entity, game, usercmd);
  player_animate(entity, game, usercmd);
}

void player_move(entity_t entity, game_t *game, const usercmd_t *usercmd)
{
  game->cdict.transform[entity].rotation += usercmd->rot * 3.0 * DELTA_TIME;
  
  vec2_t cmd_dir = vec2_init(usercmd->side, usercmd->forward);
  vec2_t wish_dir = vec2_normalize(cmd_dir);
  vec2_t rot_dir = vec2_rotate(wish_dir, game->cdict.transform[entity].rotation);
  vec2_t move_dir = vec2_mulf(rot_dir, 3.0);
  
  game->cdict.motion[entity].velocity = move_dir;
}

void player_animate(entity_t entity, game_t *game, const usercmd_t *usercmd)
{
  if (usercmd->side < 0) {
    c_animator_play(&game->cdict.animator[entity], &player_anim_move_left);
  } else if (usercmd->side > 0) {
    c_animator_play(&game->cdict.animator[entity], &player_anim_move_right);
  } else if (usercmd->forward > 0) {
    c_animator_play(&game->cdict.animator[entity], &player_anim_move_forward);
  } else if (usercmd->forward < 0) {
    c_animator_play(&game->cdict.animator[entity], &player_anim_move_back);
  }
}

void player_aim(entity_t entity, game_t *game, const usercmd_t *usercmd) {
  float player_angle = game->cdict.transform[entity].rotation; 
  float shoot_angle = player_angle - atan2(usercmd->aim_y, usercmd->aim_x);
  
  game->cdict.actor[entity].angle = shoot_angle;
  game->cdict.actor[entity].action[0].active = usercmd->attack;
}

void player_attack(entity_t entity, game_t *game)
{
  bullet_shoot(game, game->cdict.transform[entity].position, game->cdict.actor[entity].angle, 1.0, TAG_ENEMY);
}
