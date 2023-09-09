#include "game.h"

#include "system/system.h"
#include "entity/player.h"
#include "entity/orb.h"
#include "entity/warrior.h"
#include "entity/mage.h"
#include <string.h>

void game_init(game_t *game)
{
  *game = (game_t) {0};
  
  player_init(game->player, game);
  orb_spawn(game, vec2_init(3, 3));
  mage_spawn(game, vec2_init(5, 4));
  warrior_spawn(game, vec2_init(6, 2));
}

void game_update(game_t *game, const usercmd_t *usercmd)
{
  game->usercmd = *usercmd;
  
  player_update(game->player, game);
  
  sys_perform_action(game);
  sys_physics_update(game);
  sys_animate_sprite(game);
  sys_check_health(game);
}

void game_load_map(game_t *game, map_t *map)
{
  game->map = map;
}

void game_kill(game_t *game, entity_t entity)
{
  edict_kill(&game->edict, entity);
}
