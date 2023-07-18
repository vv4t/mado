#include "system.h"

void sys_perform_action(edict_t *edict)
{
  const component_t mask = COMPONENT_ACTOR;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((edict->field[i] & mask) != mask)
      continue;
    
    for (int j = 0; j < MAX_ACTION; j++) {
      if (edict->actor[i].action[j].time > 0) {
        edict->actor[i].action[j].time -= DELTA_TIME;
      } else {
        edict->actor[i].action[j].time = 0.0;
      }
      
      if (!edict->actor[i].action[j].active)
        continue;
      
      if (edict->actor[i].action[j].time <= 0) {
        edict->actor[i].action[j].time = edict->actor[i].action[j].cooldown;
        edict->actor[i].action[j].xaction(i, edict);
        edict->actor[i].action[j].count++;
      }
    }
  }
}
