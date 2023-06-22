#include "map.h"

tile_t map_get(map_t *map, int x, int y)
{
  if (x < 0 || y < 0 || x >= map->width || y >= map->height)
    return map->border_tile;
  else
    return map->data[x + y * map->width];
}

bool map_solid(map_t *map, int x, int y)
{
  tile_t tile = map_get(map, x, y);
  
  if (tile & TILE_DATA_EXISTS) {
    if (map->tile_set->tile_data[tile & ~TILE_DATA_EXISTS].solid)
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

vec2_t tile_get_uv(const tile_set_t *tile_set, tile_t tile)
{
  return vec2_init(
    tile % tile_set->sprite_sheet->sheet_width,
    tile / tile_set->sprite_sheet->sheet_height
  );
}
