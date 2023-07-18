#include "orb.h"

#include "bullet.h"

void orb_attack1(entity_t entity, edict_t *edict);
void orb_attack2(entity_t entity, edict_t *edict);

static animation_t orb_anim_idle = (animation_t) { .uv = {2,4}, .frame_count = 2, .frame_time = 0.2 };

void orb_spawn(edict_t *edict, vec2_t pos)
{
  entity_t entity = edict_spawn(edict);
  edict->field[entity] |= COMPONENT_TRANSFORM;
  edict->field[entity] |= COMPONENT_MOTION;
  edict->field[entity] |= COMPONENT_SPRITE;
  edict->field[entity] |= COMPONENT_ANIMATOR;
  edict->field[entity] |= COMPONENT_ACTOR;
  edict->field[entity] |= COMPONENT_BOX;
  edict->field[entity] |= COMPONENT_TAG;
  
  edict->transform[entity].position = pos;
  
  edict->sprite[entity].uv = vec2_init(3, 4);
  edict->sprite[entity].orient = true;
  edict->sprite[entity].stand = true;
  c_animator_play(&edict->animator[entity], &orb_anim_idle);
  
  edict->tag[entity] |= TAG_ENEMY;
  
  c_actor_set_act(&edict->actor[entity], 0, orb_attack1, 0.1);
  c_actor_set_act(&edict->actor[entity], 1, orb_attack2, 1.0);
  
  edict->box[entity].min = vec2_init(-0.2, -0.2);
  edict->box[entity].max = vec2_init(+0.2, +0.2);
}

void orb_attack1(entity_t entity, edict_t *edict)
{
  if (edict->actor[entity].action[0].count < 3) {
    vec2_t delta_pos = vec2_sub(edict->transform[0].position, edict->transform[entity].position);
    float angle = atan2(delta_pos.y, delta_pos.x);
    
    bullet_shoot(edict, edict->transform[entity].position, angle, 1.0, TAG_PLAYER);
  } else {
    edict->actor[entity].action[0].active = false;
  }
}

void orb_attack2(entity_t entity, edict_t *edict)
{
  edict->actor[entity].action[0].active = true;
  edict->actor[entity].action[0].count = 0;
}
