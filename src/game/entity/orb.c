#include "orb.h"

#include "bullet.h"
#include <stdlib.h>

#define ORB_BULLET_DAMAGE 10
#define ORB_PIVOT_RADIUS  0.4
#define ORB_PIVOT_TIME    0.25

void orb_attack1(entity_t entity, action_t *action, game_t *game);
void orb_attack2(entity_t entity, action_t *action, game_t *game);
void orb_attack3(entity_t entity, action_t *action, game_t *game);
void orb_attack4(entity_t entity, action_t *action, game_t *game);
void orb_move(entity_t entity, action_t *action, game_t *game);
void orb_die(entity_t entity, game_t *game);

static animation_t orb_anim_idle = (animation_t) { .uv = {2,4}, .frame_count = 2, .frame_time = 0.2 };

void orb_spawn(game_t *game, vec2_t pos)
{
  entity_t entity = edict_spawn(&game->edict);
  
  component_t field = 0;
  field |= c_transform_init(&game->cdict.transform[entity], pos, 0.0);
  field |= c_motion_init(&game->cdict.motion[entity]);
  field |= c_sprite_init(&game->cdict.sprite[entity], vec2_init(3,4), true, true, 0.0);
  field |= c_animator_init(&game->cdict.animator[entity]);
  field |= c_actor_init(&game->cdict.actor[entity]);
  field |= c_box_init(&game->cdict.box[entity], vec2_init(-0.2,-0.2), vec2_init(0.2, 0.2));
  field |= c_tag_init(&game->cdict.tag[entity], TAG_ENEMY);
  field |= c_health_init(&game->cdict.health[entity], 100);
  field |= c_ai_move_init(&game->cdict.ai_move[entity], vec2_add(pos, vec2_init(0, ORB_PIVOT_RADIUS)));
  game->edict.field[entity] = field;
  
  game->cdict.health[entity].xdie = orb_die;
  
  c_animator_play(&game->cdict.animator[entity], &orb_anim_idle);
  c_actor_add_act(&game->cdict.actor[entity], orb_attack2, 1.0);
  c_actor_add_act(&game->cdict.actor[entity], orb_move, ORB_PIVOT_TIME);
}

void orb_die(entity_t entity, game_t *game)
{
  game_kill(game, entity);
}

void orb_attack1(entity_t entity, action_t *action, game_t *game)
{
  if (action->count >= 2)
    c_actor_remove_act(&game->cdict.actor[entity], action);
  
  vec2_t player_pos = game->cdict.transform[0].position;
  vec2_t orb_pos = game->cdict.transform[entity].position;
  vec2_t delta_pos = vec2_sub(player_pos, orb_pos);
  
  float angle = atan2(delta_pos.y, delta_pos.x);
  
  bullet_shoot(game, orb_pos, vec2_init(1,7), angle, 1.0, TAG_PLAYER, ORB_BULLET_DAMAGE);
}

void orb_attack2(entity_t entity, action_t *action, game_t *game)
{
  c_actor_add_act(&game->cdict.actor[entity], orb_attack4, 0.09);
}

void orb_attack3(entity_t entity, action_t *action, game_t *game)
{
  if (action->count >= 2)
    c_actor_remove_act(&game->cdict.actor[entity], action);
  
  vec2_t player_pos = game->cdict.transform[0].position;
  vec2_t orb_pos = game->cdict.transform[entity].position;
  vec2_t delta_pos = vec2_sub(player_pos, orb_pos);
  
  float angle = atan2(delta_pos.y, delta_pos.x);
  
  for (int i = -1; i <= 1; i++) {
    float offset = i * M_PI / 6.0;
    bullet_shoot(game, orb_pos, vec2_init(1,7), angle + offset, 1.0, TAG_PLAYER, ORB_BULLET_DAMAGE);
  }
}

void orb_attack4(entity_t entity, action_t *action, game_t *game)
{
  if (action->count >= 5)
    c_actor_remove_act(&game->cdict.actor[entity], action);
  
  vec2_t player_pos = game->cdict.transform[0].position;
  vec2_t orb_pos = game->cdict.transform[entity].position;
  vec2_t delta_pos = vec2_sub(player_pos, orb_pos);
  
  for (int i = 0; i < 4; i++) {
    float spin = action->count * M_PI / 20.0;
    float angle = atan2(delta_pos.y, delta_pos.x) + i * M_PI / 2.0 + spin;
    
    bullet_shoot(game, orb_pos, vec2_init(1,7), angle, 1.0, TAG_PLAYER, ORB_BULLET_DAMAGE);
  }
}

void orb_move(entity_t entity, action_t *action, game_t *game)
{
  float pivot_angle = (rand() % 256) / 256.0 * M_PI * 2;
  
  vec2_t orb_pos = game->cdict.transform[entity].position;
  vec2_t pivot_pos = game->cdict.ai_move[entity].pivot;
  vec2_t delta_pos = vec2_sub(pivot_pos, orb_pos);
  vec2_t new_delta = vec2_rotate(vec2_init(0.0, ORB_PIVOT_RADIUS), pivot_angle);
  vec2_t move_delta = vec2_add(delta_pos, new_delta);
  
  float dist = vec2_length(move_delta);
  
  vec2_t move_dir = vec2_mulf(vec2_normalize(move_delta), dist / ORB_PIVOT_TIME);
  
  game->cdict.motion[entity].velocity = move_dir;
}
