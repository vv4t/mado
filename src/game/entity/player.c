#include "player.h"

#include "../system/system.h"
#include "bullet.h"

#define PLAYER_BULLET_DAMAGE 10
#define PLAYER_MOVE_SPEED 3.0
#define PLAYER_ROTATE_SPEED 3.0

static void player_move(entity_t entity, game_t *game);
static void player_animate(entity_t entity, game_t *game);
static void player_attack(entity_t entity, action_t *action, game_t *game);

static animation_t player_anim_move_right   = (animation_t) { .uv = {0,5}, .frame_count = 2, .frame_time = 0.2 };
static animation_t player_anim_move_left    = (animation_t) { .uv = {2,5}, .frame_count = 2, .frame_time = 0.2 };
static animation_t player_anim_move_forward = (animation_t) { .uv = {2,6}, .frame_count = 2, .frame_time = 0.2 };
static animation_t player_anim_move_back    = (animation_t) { .uv = {0,6}, .frame_count = 2, .frame_time = 0.2 };

static shooter_t player_shooter = {
  .uv = {0,7},
  .live_time = 0.5f,
  .target = TAG_ENEMY,
  .damage = 10,
  .speed = 8.0f
};

void player_init(entity_t entity, game_t *game)
{
  entity = edict_spawn(&game->edict);
  
  component_t field = 0;
  field |= c_transform_init(&game->cdict.transform[entity], vec2_init(2.0, 2.0), 0.0);
  field |= c_motion_init(&game->cdict.motion[entity]);
  field |= c_sprite_init(&game->cdict.sprite[entity], vec2_init(0,5));
  field |= c_animator_init(&game->cdict.animator[entity]);
  field |= c_actor_init(&game->cdict.actor[entity]);
  field |= c_box_init(&game->cdict.box[entity], vec2_init(-0.2, -0.2), vec2_init(+0.2, +0.2));
  field |= c_tag_init(&game->cdict.tag[entity], TAG_PLAYER);
  field |= c_health_init(&game->cdict.health[entity], 100);
  game->edict.field[entity] = field;
  
  c_animator_play(&game->cdict.animator[entity], &player_anim_move_left);
  c_actor_start(&game->cdict.actor[entity], player_attack, 0.125, 0);
}

void player_update(entity_t entity, game_t *game)
{
  player_move(entity, game);
  player_animate(entity, game);
}

void player_move(entity_t entity, game_t *game)
{
  game->cdict.actor[entity].action[0].active = game->usercmd.attack;
  game->cdict.transform[entity].rotation += game->usercmd.rot * PLAYER_ROTATE_SPEED * DELTA_TIME;
  
  vec2_t cmd_dir = vec2_init(game->usercmd.side, game->usercmd.forward);
  vec2_t wish_dir = vec2_normalize(cmd_dir);
  vec2_t rot_dir = vec2_rotate(wish_dir, game->cdict.transform[entity].rotation);
  vec2_t move_dir = vec2_mulf(rot_dir, PLAYER_MOVE_SPEED);
  
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

void player_attack(entity_t entity, action_t *action, game_t *game)
{
  vec2_t pos = game->cdict.transform[entity].position;
  
  float player_angle = game->cdict.transform[entity].rotation; 
  float angle = player_angle - atan2(game->usercmd.aim_y, game->usercmd.aim_x);
  
  bullet_shoot(game, &player_shooter, pos, angle);
}
