#include "system.h"

void sys_perform_action(game_t *game)
{
  const component_t mask = COMPONENT_ACTOR;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((game->edict.field[i] & mask) != mask)
      continue;
    
    c_actor_t *c_actor = &game->cdict.actor[i];
    
    for (int j = 0; j < MAX_ACTION; j++) {
      if (c_actor->action[j].time > 0) {
        c_actor->action[j].time -= DELTA_TIME;
      } else {
        c_actor->action[j].time = 0.0;
      }
      
      if (!c_actor->action[j].active)
        continue;
      
      if (c_actor->action[j].time <= 0) {
        c_actor->action[j].time = c_actor->action[j].cooldown;
        c_actor->action[j].xaction(i, game);
        c_actor->action[j].count++;
      }
    }
  }
}
