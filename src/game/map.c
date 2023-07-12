#include "map.h"

tile_t map_get(const map_t *map, int x, int y)
{
  if (x < 0 || y < 0 || x >= map->width || y >= map->height)
    return map->border_tile;
  else
    return map->data[x + y * map->width];
}

bool map_solid(const map_t *map, int x, int y)
{
  tile_t tile = map_get(map, x, y);
  
  if (tile & TILE_DATA_EXISTS) {
    if (map->tile_set->tile_data[tile & ~TILE_DATA_EXISTS].solid)
      return true;
  }
  
  return false;
}

bool map_collide(const map_t *map, vec2_t pos, vec2_t min, vec2_t max)
{
  int x0 = floor(pos.x + min.x);
  int y0 = floor(pos.y + min.y);
  int x1 = floor(pos.x + max.x);
  int y1 = floor(pos.y + max.y);
  
  if (map_solid(map, x0, y0)) return true;
  if (map_solid(map, x1, y0)) return true;
  if (map_solid(map, x0, y1)) return true;
  if (map_solid(map, x1, y1)) return true;
  
  return false;
}

tile_data_t *tile_get_data(tile_set_t *tile_set, tile_t tile)
{
  if (tile & TILE_DATA_EXISTS) {
    return &tile_set->tile_data[tile & ~TILE_DATA_EXISTS];
  }
  
  return NULL;
}

vec2_t tile_get_uv(const tile_set_t *tile_set, tile_t tile)
{
  return vec2_init(
    tile % tile_set->sprite_sheet->sheet_width,
    tile / tile_set->sprite_sheet->sheet_height
  );
}
