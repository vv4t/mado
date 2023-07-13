#include "game.h"
#include "../common/time.h"

void game_init(game_t *game)
{
  *game = (game_t) {0};
  
  player_init(&game->player, &game->edict);
  orb_spawn(&game->edict, vec2_init(3, 3));
}

void game_update(game_t *game, const usercmd_t *usercmd)
{
  game->delta_time = (time_ms_since_epoch() - game->time) / 1000.0;
  game->time = time_ms_since_epoch();

  game->animation_time += 0.015;
  
  player_update(&game->player, &game->edict, usercmd);
  
  perform_act(&game->edict);
  
  predict_motion(&game->edict);
  box_hit_map(&game->edict, game->map);
  box_hit_box(&game->edict);
  predict_motion(&game->edict);
  integrate_motion(&game->edict);
  
  animate_sprite(&game->edict);
}

void game_load_map(game_t *game, map_t *map)
{
  game->map = map;
}
