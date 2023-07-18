#include "game.h"

#include "system/system.h"
#include "entity/player.h"
#include "entity/orb.h"

void game_init(game_t *game)
{
  *game = (game_t) {0};
  
  player_init(game->player, game);
  orb_spawn(game, vec2_init(3, 3));
}

void game_update(game_t *game, const usercmd_t *usercmd)
{
  player_update(game->player, game, usercmd);
  
  sys_perform_action(game);
  sys_physics_update(game);
  sys_animate_sprite(game);
}

void game_load_map(game_t *game, map_t *map)
{
  game->map = map;
}
