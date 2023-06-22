#include "game.h"

void player_init(player_t *player, sprite_t *sprite);
void player_move(player_t *player, map_t *map, float time, const usercmd_t *usercmd);

tile_t map_get(map_t *map, int x, int y);
bool map_solid(map_t *map, int x, int y);
bool map_collide(map_t *map, vec2_t pos, vec2_t box);

void game_init(game_t *game)
{
  *game = (game_t) { 0 };
  
  game->sprites[0].pos = vec2_init(2.0, 2.0);
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

void player_init(player_t *player, sprite_t *sprite)
{
  player->pos = vec2_init(0.0, 0.0);
  player->sprite = sprite;
}

void player_move(player_t *player, map_t *map, float time, const usercmd_t *usercmd)
{
  player->rot += usercmd->rot * 0.05;
  
  vec2_t wish_dir = vec2_init(usercmd->side, usercmd->forward);
  vec2_t move_dir = vec2_mulf(vec2_rotate(wish_dir, player->rot), 0.05);
  
  vec2_t new_pos = vec2_add(player->pos, move_dir);
  vec2_t new_pos_x = vec2_add(player->pos, vec2_init(move_dir.x, 0.0));
  vec2_t new_pos_y = vec2_add(player->pos, vec2_init(0.0, move_dir.y));
  vec2_t box = vec2_init(0.2, 0.2);
  
  if (map_collide(map, new_pos, box)) {
    if (!map_collide(map, new_pos_y, box)) {
      player->pos = new_pos_y;
    } else if (!map_collide(map, new_pos_x, box)) {
      player->pos = new_pos_x;
    } 
  } else {
    player->pos = new_pos;
  }
  
  if (fabs(move_dir.x) + fabs(move_dir.y) > 0.04)
    player->sprite->uv.x = floor(cos(time * 8) * cos(time * 8) * 2);
  
  player->sprite->pos = player->pos;
}

tile_t map_get(map_t *map, int x, int y)
{
  return map->data[x + y * map->width];
}

bool map_solid(map_t *map, int x, int y)
{
  tile_t tile = map_get(map, x, y);
  
  if (tile & TILE_DATA_EXISTS) {
    if (map->sprite_sheet->tile_data[tile & ~TILE_DATA_EXISTS].solid)
      return true;
  }
  
  return false;
}

bool map_collide(map_t *map, vec2_t pos, vec2_t box)
{
  int x0 = floor(pos.x - box.x);
  int y0 = floor(pos.y - box.y);
  int x1 = floor(pos.x + box.x);
  int y1 = floor(pos.y + box.y);
  
  if (map_solid(map, x0, y0)) return true;
  if (map_solid(map, x1, y0)) return true;
  if (map_solid(map, x0, y1)) return true;
  if (map_solid(map, x1, y1)) return true;
  
  return false;
}


