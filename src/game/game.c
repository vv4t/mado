#include "game.h"

void game_init(game_t *game)
{
  game->pos = vec2_init(0.0, 0.0);
}

void game_update(game_t *game, const usercmd_t *usercmd)
{
  game->rot += usercmd->rot * 0.015;
  
  vec2_t wish_dir = vec2_init(usercmd->side, usercmd->forward);
  vec2_t move_dir = vec2_mulf(vec2_rotate(wish_dir, game->rot), 0.015);
  
  game->pos = vec2_add(game->pos, move_dir);
}
