#include "system.h"

void xhit_bullet(entity_t entity, edict_t *edict, entity_t hit)
{
  edict_kill(edict, entity);
}

void shoot_bullet(edict_t *edict, vec2_t pos, float angle, float live_time)
{
  entity_t entity = edict_spawn(edict);
  edict->field[entity] |= COMPONENT_TRANSFORM;
  edict->field[entity] |= COMPONENT_MOTION;
  edict->field[entity] |= COMPONENT_SPRITE;
  edict->field[entity] |= COMPONENT_BULLET;
  edict->field[entity] |= COMPONENT_BOX;
  
  edict->transform[entity].position = pos;
  edict->transform[entity].rotation = angle;
  
  edict->sprite[entity].uv = vec2_init(0, 7);
  edict->sprite[entity].rotation = angle - M_PI/2.0;
  edict->sprite[entity].orient = false;
  edict->sprite[entity].stand = false;
  
  edict->motion[entity].velocity = vec2_init(cos(angle) * 10, sin(angle) * 10);
  edict->box[entity].min = vec2_init(-0.2, -0.2);
  edict->box[entity].max = vec2_init(+0.2, +0.2);
  edict->box[entity].xhit = xhit_bullet;
  
  edict->bullet[entity].live_time = live_time;
}

void perform_act(edict_t *edict)
{
  const component_t mask = COMPONENT_ACTOR;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((edict->field[i] & mask) != mask)
      continue;
    
    for (int j = 0; j < edict->actor[i].num_act; j++) {
      if (!edict->actor[i].act[j].active)
        continue;
      
      edict->actor[i].act[j].time -= DELTA_TIME;
      
      if (edict->actor[i].act[j].time <= 0) {
        edict->actor[i].act[j].time = edict->actor[i].act[j].cooldown;
        edict->actor[i].act[j].xaction(i, edict);
        edict->actor[i].act[j].count++;
      }
    }
  }
}

void decay_bullet(edict_t *edict) {
  const component_t mask = COMPONENT_BULLET;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((edict->field[i] & mask) != mask)
      continue;
    
    edict->bullet[i].live_time -= 0.015;
    
    if (edict->bullet[i].live_time <= 0) {
      edict_kill(edict, i);
    }
    
    if (edict->box[i].hit_map) {
      edict_kill(edict, i);
    }
  }
}
