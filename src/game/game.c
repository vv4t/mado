#include "game.h"

void game_init(game_t *game)
{
  *game = (game_t) { 0 };
  
  game->sprites[0].pos = vec2_init(0.0, 0.0);
  game->sprites[0].uv = vec2_init(0, 5);
  game->sprites[0].show = true;
  game->sprites[0].stand = true;
  
  player_init(&game->player, &game->sprites[0]);
}

void game_update(game_t *game, const usercmd_t *usercmd)
{
  game->time += 0.015;
  player_move(&game->player, game->map, game->time, usercmd);
}

void game_load_map(game_t *game, map_t *map)
{
  game->map = map;
}
