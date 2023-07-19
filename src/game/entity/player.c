#include "player.h"

#include "../system/system.h"
#include "bullet.h"

static void player_move(entity_t entity, game_t *game);
static void player_animate(entity_t entity, game_t *game);
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
  game->edict.field[entity] |= COMPONENT_HEALTH;
  
  game->cdict.tag[entity] |= TAG_PLAYER;
  game->cdict.transform[entity].position = vec2_init(2.0, 2.0);
  game->cdict.health[entity].health = 100;
  
  c_box_init(&game->cdict.box[entity], vec2_init(-0.2, -0.2), vec2_init(+0.2, +0.2));
  c_sprite_init(&game->cdict.sprite[entity], vec2_init(0, 5), true, true, 0.0);
  c_animator_play(&game->cdict.animator[entity], &player_anim_move_left);
  c_actor_set_act(&game->cdict.actor[entity], 0, player_attack, 0.25);
}

void player_update(entity_t entity, game_t *game)
{
  player_move(entity, game);
  player_animate(entity, game);
}

void player_move(entity_t entity, game_t *game)
{
  game->cdict.actor[entity].action[0].active = game->usercmd.attack;
  game->cdict.transform[entity].rotation += game->usercmd.rot * 3.0 * DELTA_TIME;
  
  vec2_t cmd_dir = vec2_init(game->usercmd.side, game->usercmd.forward);
  vec2_t wish_dir = vec2_normalize(cmd_dir);
  vec2_t rot_dir = vec2_rotate(wish_dir, game->cdict.transform[entity].rotation);
  vec2_t move_dir = vec2_mulf(rot_dir, 3.0);
  
  game->cdict.motion[entity].velocity = move_dir;
}

void player_animate(entity_t entity, game_t *game)
{
  if (game->usercmd.side < 0) {
    c_animator_play(&game->cdict.animator[entity], &player_anim_move_left);
  } else if (game->usercmd.side > 0) {
    c_animator_play(&game->cdict.animator[entity], &player_anim_move_right);
  } else if (game->usercmd.forward > 0) {
    c_animator_play(&game->cdict.animator[entity], &player_anim_move_forward);
  } else if (game->usercmd.forward < 0) {
    c_animator_play(&game->cdict.animator[entity], &player_anim_move_back);
  }
}

void player_attack(entity_t entity, game_t *game)
{
  vec2_t pos = game->cdict.transform[entity].position;
  
  float player_angle = game->cdict.transform[entity].rotation; 
  float angle = player_angle - atan2(game->usercmd.aim_y, game->usercmd.aim_x);
  
  bullet_shoot(game, pos, vec2_init(0,7), angle, 1.0, TAG_ENEMY);
}
