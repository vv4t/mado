#include "game.h"

void player_init(player_t *player, sprite_t *sprite);
void player_move(player_t *player, const usercmd_t *usercmd);

void game_init(game_t *game)
{
  *game = (game_t) { 0 };
  
  game->sprites[0].pos = vec2_init(0.0, 2.0);
  game->sprites[0].show = true;
  game->sprites[0].stand = true;
  
  player_init(&game->player, &game->sprites[0]);
}

void game_update(game_t *game, const usercmd_t *usercmd)
{
  player_move(&game->player, usercmd);
}

void player_init(player_t *player, sprite_t *sprite)
{
  player->pos = vec2_init(0.0, 0.0);
  player->sprite = sprite;
}

void player_move(player_t *player, const usercmd_t *usercmd)
{
  player->rot += usercmd->rot * 0.015;
  
  vec2_t wish_dir = vec2_init(usercmd->side, usercmd->forward);
  vec2_t move_dir = vec2_mulf(vec2_rotate(wish_dir, player->rot), 0.015);
  
  player->pos = vec2_add(player->pos, move_dir);
  
  player->sprite->pos = player->pos;
}
