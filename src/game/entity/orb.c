#include "orb.h"

#include "bullet.h"

void orb_attack1(entity_t entity, game_t *game);
void orb_attack2(entity_t entity, game_t *game);

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
  
  game->cdict.transform[entity].position = pos;
  
  game->cdict.sprite[entity].uv = vec2_init(3, 4);
  game->cdict.sprite[entity].orient = true;
  game->cdict.sprite[entity].stand = true;
  c_animator_play(&game->cdict.animator[entity], &orb_anim_idle);
  
  game->cdict.tag[entity] |= TAG_ENEMY;
  
  c_actor_set_act(&game->cdict.actor[entity], 0, orb_attack1, 0.1);
  c_actor_set_act(&game->cdict.actor[entity], 1, orb_attack2, 1.0);
  
  game->cdict.box[entity].min = vec2_init(-0.2, -0.2);
  game->cdict.box[entity].max = vec2_init(+0.2, +0.2);
}

void orb_attack1(entity_t entity, game_t *game)
{
  if (game->cdict.actor[entity].action[0].count < 3) {
    vec2_t delta_pos = vec2_sub(game->cdict.transform[0].position, game->cdict.transform[entity].position);
    float angle = atan2(delta_pos.y, delta_pos.x);
    
    bullet_shoot(game, game->cdict.transform[entity].position, angle, 1.0, TAG_PLAYER);
  } else {
    game->cdict.actor[entity].action[0].active = false;
  }
}

void orb_attack2(entity_t entity, game_t *game)
{
  game->cdict.actor[entity].action[0].active = true;
  game->cdict.actor[entity].action[0].count = 0;
}
