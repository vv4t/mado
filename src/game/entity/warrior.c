#include "warrior.h"

#include "bullet.h"
#include <stdlib.h>

#define WARRIOR_BULLET_DAMAGE 10
#define WARRIOR_PIVOT_RADIUS  0.4
#define WARRIOR_PIVOT_TIME    0.25

void warrior_attack(entity_t entity, action_t *action, game_t *game);
void warrior_shotgun(entity_t entity, action_t *action, game_t *game);
void warrior_move(entity_t entity, action_t *action, game_t *game);
void warrior_die(entity_t entity, game_t *game);

static animation_t warrior_anim_idle = (animation_t) { .uv = {0,3}, .frame_count = 2, .frame_time = 0.5 };
static animation_t warrior_anim_attack = (animation_t) { .uv = {2,3}, .frame_count = 2, .frame_time = 0.5 };

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
  
  game->cdict.sprite[entity].size = vec2_init(1.5, 1.5);
  
  game->cdict.health[entity].xdie = warrior_die;
  
  c_animator_play(&game->cdict.animator[entity], &warrior_anim_idle);
  c_actor_add_act(&game->cdict.actor[entity], warrior_attack, 2.5);
  c_actor_add_act(&game->cdict.actor[entity], warrior_move, WARRIOR_PIVOT_TIME);
}

void warrior_die(entity_t entity, game_t *game)
{
  game_kill(game, entity);
}

void warrior_attack(entity_t entity, action_t *action, game_t *game)
{
  c_animator_play(&game->cdict.animator[entity], &warrior_anim_attack);
  c_actor_add_act(&game->cdict.actor[entity], warrior_shotgun, 0.5);
}

void warrior_shotgun(entity_t entity, action_t *action, game_t *game)
{
  if (action->count == 1) {
    vec2_t player_pos = game->cdict.transform[0].position;
    vec2_t warrior_pos = game->cdict.transform[entity].position;
    vec2_t delta_pos = vec2_sub(player_pos, warrior_pos);
    
    float angle = atan2(delta_pos.y, delta_pos.x);
    
    bullet_shoot(game, warrior_pos, vec2_init(3,7), angle, 1.0, TAG_PLAYER, WARRIOR_BULLET_DAMAGE);
    bullet_shoot(game, warrior_pos, vec2_init(3,7), angle - 0.5, 1.0, TAG_PLAYER, WARRIOR_BULLET_DAMAGE);
    bullet_shoot(game, warrior_pos, vec2_init(3,7), angle + 0.5, 1.0, TAG_PLAYER, WARRIOR_BULLET_DAMAGE);
  } else if (action->count >= 2) {
    c_actor_remove_act(&game->cdict.actor[entity], action);
    c_animator_play(&game->cdict.animator[entity], &warrior_anim_idle);
  }
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
