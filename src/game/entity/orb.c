#include "orb.h"

#include "bullet.h"

#define ACT_ORB_ATTACK_1 0
#define ACT_ORB_ATTACK_2 1
#define ORB_BULLET_DAMAGE 10

void orb_attack1(entity_t entity, game_t *game);
void orb_attack2(entity_t entity, game_t *game);
void orb_die(entity_t entity, game_t *game);

static animation_t orb_anim_idle = (animation_t) { .uv = {2,4}, .frame_count = 2, .frame_time = 0.2 };

void orb_spawn(game_t *game, vec2_t pos)
{
  entity_t entity = edict_spawn(&game->edict);
  game->edict.field[entity] |= COMPONENT_TRANSFORM;
  game->edict.field[entity] |= COMPONENT_MOTION;
  game->edict.field[entity] |= COMPONENT_SPRITE;
  game->edict.field[entity] |= COMPONENT_ANIMATOR;
  game->edict.field[entity] |= COMPONENT_ACTOR;
  game->edict.field[entity] |= COMPONENT_BOX;
  game->edict.field[entity] |= COMPONENT_TAG;
  game->edict.field[entity] |= COMPONENT_HEALTH;
  
  game->cdict.health[entity].health = 100;
  game->cdict.health[entity].xdie = orb_die;
  
  game->cdict.transform[entity].position = pos;
  game->cdict.tag[entity] = TAG_ENEMY;
  
  c_box_init(&game->cdict.box[entity], vec2_init(-0.2, -0.2), vec2_init(+0.2, +0.2));
  c_sprite_init(&game->cdict.sprite[entity], vec2_init(3, 4), true, true, 0.0);
  c_animator_play(&game->cdict.animator[entity], &orb_anim_idle);
  c_actor_set_act(&game->cdict.actor[entity], ACT_ORB_ATTACK_1, orb_attack1, 0.1);
  c_actor_set_act(&game->cdict.actor[entity], ACT_ORB_ATTACK_2, orb_attack2, 1.0);
}

void orb_die(entity_t entity, game_t *game)
{
  edict_kill(&game->edict, entity);
}

void orb_attack1(entity_t entity, game_t *game)
{
  action_t *attack1 = &game->cdict.actor[entity].action[ACT_ORB_ATTACK_1];
  
  if (attack1->count < 3) {
    vec2_t player_pos = game->cdict.transform[0].position;
    vec2_t orb_pos = game->cdict.transform[entity].position;
    vec2_t delta_pos = vec2_sub(player_pos, orb_pos);
    
    float angle = atan2(delta_pos.y, delta_pos.x);
    
    bullet_shoot(game, orb_pos, vec2_init(1,7), angle, 1.0, TAG_PLAYER, ORB_BULLET_DAMAGE);
  } else {
    attack1->active = false;
  }
}

void orb_attack2(entity_t entity, game_t *game)
{
  game->cdict.actor[entity].action[ACT_ORB_ATTACK_1].active = true;
  game->cdict.actor[entity].action[ACT_ORB_ATTACK_1].count = 0;
}
