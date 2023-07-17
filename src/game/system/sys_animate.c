#include "system.h"

void sys_animate_sprite(edict_t *edict)
{
  const component_t mask = COMPONENT_SPRITE | COMPONENT_ANIMATOR;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((edict->field[i] & mask) != mask)
      continue;
    
    if (!edict->animator[i].animation)
      continue;
    
    edict->animator[i].time += DELTA_TIME / edict->animator[i].animation->frame_time;
    
    int frame_id = ((int) edict->animator[i].time) % edict->animator[i].animation->frame_count;
    
    edict->sprite[i].uv.x = edict->animator[i].animation->uv.x + frame_id;
    edict->sprite[i].uv.y = edict->animator[i].animation->uv.y;
  }
}
