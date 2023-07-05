#include "system.h"

void shoot_bullet(edict_t *edict, vec2_t pos, float angle, float live_time) {
  entity_t entity = edict_spawn(edict);
  edict->field[entity] |= COMPONENT_TRANSFORM;
  edict->field[entity] |= COMPONENT_MOTION;
  edict->field[entity] |= COMPONENT_SPRITE;
  edict->field[entity] |= COMPONENT_BULLET;
  
  edict->transform[entity].position = pos;
  edict->transform[entity].rotation = angle;
  
  edict->sprite[entity].uv = vec2_init(0, 7);
  edict->sprite[entity].rotation = angle - M_PI/2.0;
  edict->sprite[entity].orient = false;
  
  edict->motion[entity].velocity = vec2_init(cos(angle) * 10, sin(angle) * 10);
  edict->bullet[entity].live_time = live_time;
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
  }
}
