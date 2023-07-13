#include "system.h"

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
  play_animation(&edict->animator[entity], &orb_anim_idle);
  
  edict->tag[entity] |= TAG_ENEMY;
  
  edict->actor[entity].act[0].xaction = orb_attack1;
  edict->actor[entity].act[0].time = 0.0;
  edict->actor[entity].act[0].cooldown = 0.1;
  edict->actor[entity].act[0].active = true;
  
  edict->actor[entity].act[1].xaction = orb_attack2;
  edict->actor[entity].act[1].time = 0.0;
  edict->actor[entity].act[1].cooldown = 1.0;
  edict->actor[entity].act[1].active = true;
  
  edict->box[entity].min = vec2_init(-0.2, -0.2);
  edict->box[entity].max = vec2_init(+0.2, +0.2);
  
  edict->actor[entity].num_act = 2;
}

void orb_attack1(entity_t entity, edict_t *edict)
{
  if (edict->actor[entity].act[0].count < 3) {
    vec2_t delta_pos = vec2_sub(edict->transform[0].position, edict->transform[entity].position);
    float angle = atan2(delta_pos.y, delta_pos.x);
    
    shoot_bullet(edict, edict->transform[entity].position, angle, 1.0, TAG_PLAYER);
  } else {
    edict->actor[entity].act[0].active = false;
  }
}

void orb_attack2(entity_t entity, edict_t *edict)
{
  edict->actor[entity].act[0].active = true;
  edict->actor[entity].act[0].count = 0;
}
