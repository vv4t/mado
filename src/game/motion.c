#include "edict.h"

#include "system.h"

void integrate_motion(edict_t *edict)
{
  const component_t mask = COMPONENT_TRANSFORM | COMPONENT_MOTION;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((edict->field[i] & mask) != mask)
      continue;
    
    edict->transform[i].position = edict->motion[i].new_pos;
  }
}

void predict_motion(edict_t *edict)
{
  const component_t mask = COMPONENT_TRANSFORM | COMPONENT_MOTION;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((edict->field[i] & mask) != mask)
      continue;
    
    vec2_t delta_pos = vec2_mulf(edict->motion[i].velocity, DELTA_TIME);
    vec2_t new_pos = vec2_add(edict->transform[i].position, delta_pos);
    
    edict->motion[i].new_pos = new_pos;
  }
}

void box_hit_box(edict_t *edict)
{
  const component_t mask = COMPONENT_TRANSFORM | COMPONENT_MOTION | COMPONENT_BOX;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((edict->field[i] & mask) != mask)
      continue;
    
    vec2_t a_min = vec2_add(edict->motion[i].new_pos, edict->box[i].min);
    vec2_t a_max = vec2_add(edict->motion[i].new_pos, edict->box[i].max);
    
    for (int j = i + 1; j < MAX_ENTITIES; j++) {
      if ((edict->field[j] & mask) != mask)
        continue;
      
      vec2_t b_min = vec2_add(edict->motion[j].new_pos, edict->box[j].min);
      vec2_t b_max = vec2_add(edict->motion[j].new_pos, edict->box[j].max);
      
      if (
        a_min.x <= b_max.x &&
        a_max.x >= b_min.x &&
        a_min.y <= b_max.y &&
        a_max.y >= b_min.y
      ) {
        if (edict->box[i].xhit)
          edict->box[i].xhit(i, edict, j);
        
        if (edict->box[j].xhit)
          edict->box[j].xhit(j, edict, i);
      }
    }
  }
}
    

void box_hit_map(edict_t *edict, const map_t *map)
{
  const component_t mask = COMPONENT_TRANSFORM | COMPONENT_MOTION | COMPONENT_BOX;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((edict->field[i] & mask) != mask)
      continue;
    
    vec2_t old_pos = edict->transform[i].position;
    vec2_t new_pos = edict->motion[i].new_pos;
    
    vec2_t new_pos_x = vec2_init(new_pos.x, old_pos.y);
    vec2_t new_pos_y = vec2_init(old_pos.x, new_pos.y);
    
    vec2_t min = edict->box[i].min;
    vec2_t max = edict->box[i].max;
    
    edict->box[i].hit_map = false;
    
    if (map_collide(map, new_pos, min, max)) {
      if (!map_collide(map, new_pos_y, min, max)) {
        edict->motion[i].velocity.x = 0.0;
      } else if (!map_collide(map, new_pos_x, min, max)) {
        edict->motion[i].velocity.y = 0.0;
      } else {
        edict->motion[i].velocity.x = 0.0;
        edict->motion[i].velocity.y = 0.0;
      }
      
      edict->box[i].hit_map = true;
    }
  }
}
