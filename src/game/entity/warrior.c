#include "warrior.h"

#include "bullet.h"
#include <stdlib.h>

#define WARRIOR_BULLET_DAMAGE 10
#define WARRIOR_PIVOT_RADIUS  0.2
#define WARRIOR_PIVOT_TIME    0.2

void warrior_attack(entity_t entity, action_t *action, game_t *game);
void warrior_shotgun(entity_t entity, action_t *action, game_t *game);
void warrior_move(entity_t entity, action_t *action, game_t *game);
void warrior_die(entity_t entity, game_t *game);

static animation_t warrior_anim_idle = { .uv = {0,3}, .frame_count = 2, .frame_time = 0.3 };
static animation_t warrior_anim_attack = { .uv = {4,3}, .frame_count = 2, .frame_time = 0.3 };

static shooter_t warrior_shooter = {
  .uv = {3,7},
  .target = TAG_PLAYER,
  .damage = 10,
  .live_time = 1.0f,
  .speed = 10.0f
};

void warrior_spawn(game_t *game, vec2_t pos)
{
  entity_t entity = edict_spawn(&game->edict);
  
  component_t field = 0;
  field |= c_transform_init(&game->cdict.transform[entity], pos, 0.0);
  field |= c_motion_init(&game->cdict.motion[entity]);
  field |= c_sprite_init(&game->cdict.sprite[entity], vec2_init(0,3));
  field |= c_animator_init(&game->cdict.animator[entity]);
  field |= c_actor_init(&game->cdict.actor[entity]);
  field |= c_box_init(&game->cdict.box[entity], vec2_init(-0.2,-0.2), vec2_init(0.2, 0.2));
  field |= c_tag_init(&game->cdict.tag[entity], TAG_ENEMY);
  field |= c_health_init(&game->cdict.health[entity], 100);
  field |= c_ai_move_init(&game->cdict.ai_move[entity], vec2_add(pos, vec2_init(0, WARRIOR_PIVOT_RADIUS)));
  game->edict.field[entity] = field;
  
  game->cdict.sprite[entity].size = vec2_init(3.0, 1.5);
  game->cdict.sprite[entity].spr_size = vec2_init(2.0, 1.0);
  game->cdict.health[entity].xdie = warrior_die;
  
  c_animator_repeat(&game->cdict.animator[entity], &warrior_anim_idle);
  c_actor_start(&game->cdict.actor[entity], warrior_attack, 2.5, 0);
  c_actor_start(&game->cdict.actor[entity], warrior_move, WARRIOR_PIVOT_TIME, 0);
}

void warrior_die(entity_t entity, game_t *game)
{
  game_kill(game, entity);
}

void warrior_attack(entity_t entity, action_t *action, game_t *game)
{
  c_animator_play(&game->cdict.animator[entity], &warrior_anim_attack);
  c_actor_start(&game->cdict.actor[entity], warrior_shotgun, 0.3, 1);
}

void warrior_shotgun(entity_t entity, action_t *action, game_t *game)
{
  vec2_t player_pos = game->cdict.transform[0].position;
  vec2_t warrior_pos = game->cdict.transform[entity].position;
  vec2_t delta_pos = vec2_sub(player_pos, warrior_pos);
  
  float angle = atan2(delta_pos.y, delta_pos.x);
  
  bullet_shoot(game, &warrior_shooter, warrior_pos, angle);
  bullet_shoot(game, &warrior_shooter, warrior_pos, angle - 0.5);
  bullet_shoot(game, &warrior_shooter, warrior_pos, angle + 0.5);
}

void warrior_move(entity_t entity, action_t *action, game_t *game)
{
  float pivot_angle = (rand() % 256) / 256.0 * M_PI * 2;
  
  vec2_t warrior_pos = game->cdict.transform[entity].position;
  vec2_t pivot_pos = game->cdict.ai_move[entity].pivot;
  vec2_t delta_pos = vec2_sub(pivot_pos, warrior_pos);
  vec2_t new_delta = vec2_rotate(vec2_init(0.0, WARRIOR_PIVOT_RADIUS), pivot_angle);
  vec2_t move_delta = vec2_add(delta_pos, new_delta);
  
  float dist = vec2_length(move_delta);
  
  vec2_t move_dir = vec2_mulf(vec2_normalize(move_delta), dist / WARRIOR_PIVOT_TIME);
  
  game->cdict.motion[entity].velocity = move_dir;
}
