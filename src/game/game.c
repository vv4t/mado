#include "game.h"

void game_init(game_t *game)
{
  *game = (game_t) { 0 };
  
  player_init(&game->player, &game->sprites[1]);
  orb_init(&game->orbs[0], &game->sprites[0]);
}

void game_update(game_t *game, const usercmd_t *usercmd)
{
  game->time += 0.015;
  player_move(&game->player, game->map, game->time, usercmd);
  
  orb_update(&game->orbs[0], game->time);
}

void game_load_map(game_t *game, map_t *map)
{
  game->map = map;
}
