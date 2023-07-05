#include "game.h"
#include "../common/time.h"

void game_init(game_t *game)
{
  *game = (game_t) { 0 };
  
  game->time = time_ms_since_epoch();

  sprite_t *new_sprite = sprite_alloc(game->sprites);
  new_sprite->pos = vec2_init(0.0, 0.0);
  new_sprite->uv = vec2_init(0, 5);
  new_sprite->show = true;
  new_sprite->stand = true;
  new_sprite->used = true;
  
  player_init(&game->player, new_sprite);

}

void game_update(game_t *game, const usercmd_t *usercmd)
{
  game->delta_time = (time_ms_since_epoch() - game->time) / 1000.0;
  game->time = time_ms_since_epoch();

  game->animation_time += 0.015;
  
  player_move(
    &game->player, 
    game->map, 
    game->animation_time, 
    game->delta_time, 
    usercmd
  );

  player_shoot(
    &game->player, 
    game->bullets, 
    game->sprites, 
    game->delta_time, 
    usercmd
  );

  for (int i = 0; i < MAX_BULLETS; i++) {
    if (game->bullets[i].used) {
      bullet_update(&(game->bullets[i]), game->delta_time);
    }
  }
}

void game_load_map(game_t *game, map_t *map)
{
  game->map = map;
}
