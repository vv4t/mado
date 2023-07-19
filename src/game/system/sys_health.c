#include "system.h"

void sys_check_health(game_t *game)
{
  const component_t mask = COMPONENT_HEALTH;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((game->edict.field[i] & mask) != mask)
      continue;
    
    if (game->cdict.health[i].health < 0) {
      if (game->cdict.health[i].xdie) {
        game->cdict.health[i].xdie(i, game);
      }
    }
  }
}
