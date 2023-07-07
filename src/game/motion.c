#include "edict.h"

#include "system.h"

void integrate_motion(edict_t *edict)
{
  const int c_mask = COMPONENT_TRANSFORM | COMPONENT_MOTION;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((edict->field[i] & c_mask) != c_mask)
      continue;
    
    vec2_t delta_pos = vec2_mulf(edict->motion[i].velocity, DELTA_TIME);
    vec2_t new_pos = vec2_add(edict->transform[i].position, delta_pos);
    
    edict->transform[i].position = new_pos;
  }
}

void clip_motion(edict_t *edict, const map_t *map)
{
  const int c_mask = COMPONENT_TRANSFORM | COMPONENT_MOTION;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((edict->field[i] & c_mask) != c_mask)
      continue;
    
    vec2_t old_pos = edict->transform[i].position;
    vec2_t delta_pos = vec2_mulf(edict->motion[i].velocity, DELTA_TIME);
    vec2_t new_pos = vec2_add(old_pos, delta_pos);
    
    vec2_t new_pos_x = vec2_add(old_pos, vec2_init(edict->motion[i].velocity.x, 0.0));
    vec2_t new_pos_y = vec2_add(old_pos, vec2_init(0.0, edict->motion[i].velocity.y));
    vec2_t box = vec2_init(0.2, 0.2);
    
    edict->motion[i].hit_map = false;
    
    if (map_collide(map, new_pos.x, new_pos.y, box)) {
      if (!map_collide(map, edict->transform[i].position.x, new_pos.y, box)) {
        edict->motion[i].velocity.x = 0.0;
      } else if (!map_collide(map, new_pos.x, edict->transform[i].position.y, box)) {
        edict->motion[i].velocity.y = 0.0;
      } else {
        edict->motion[i].velocity.x = 0.0;
        edict->motion[i].velocity.y = 0.0;
      }
      
      edict->motion[i].hit_map = true;
    }
  }
}
