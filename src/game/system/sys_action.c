#include "system.h"

void sys_perform_action(game_t *game)
{
  const component_t mask = COMPONENT_ACTOR;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((game->edict.field[i] & mask) != mask)
      continue;
    
    c_actor_t *c_actor = &game->cdict.actor[i];
    
    for (int j = 0; j < MAX_ACTION; j++) {
      action_t *action = &c_actor->action[j];
      
      if (!action->xaction) {
        continue;
      }
      
      action->time = fmax(action->time - DELTA_TIME, 0.0);
      
      if (!action->active) {
        continue;
      }
      
      if (action->time <= 0) {
        action->time = action->cooldown;
        action->xaction(i, action, game);
      
        if (action->count > 0) {
          action->count--;
          
          if (action->count == 0) {
            c_actor_stop(&game->cdict.actor[i], j);
          }
        }
      }
    }
  }
}
