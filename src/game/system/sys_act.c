#include "system.h"

void sys_perform_act(edict_t *edict)
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
