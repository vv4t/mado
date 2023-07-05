#include "game.h"
#include "../common/time.h"

void game_init(game_t *game)
{
  *game = (game_t) {0};
  
  player_init(&game->player, &game->edict);
}

void game_update(game_t *game, const usercmd_t *usercmd)
{
  game->delta_time = (time_ms_since_epoch() - game->time) / 1000.0;
  game->time = time_ms_since_epoch();

  game->animation_time += 0.015;
  
  player_update(&game->player, &game->edict, usercmd);
  
  decay_bullet(&game->edict);
  clip_motion(&game->edict, game->map);
  integrate_motion(&game->edict);
  animate_sprite(&game->edict);
}

void game_load_map(game_t *game, map_t *map)
{
  game->map = map;
}
