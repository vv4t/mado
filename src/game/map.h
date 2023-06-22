#ifndef MAP_H
#define MAP_H

#include "sprite.h"

#define MAX_BLOCKS 4

typedef enum {
  TILE_DATA_EXISTS = (1 << 31)
} tile_t;

typedef struct {
  tile_t block[MAX_BLOCKS];
  int num_block;
  bool solid;
} tile_data_t;

typedef struct {
  sprite_sheet_t *sprite_sheet;
  tile_data_t *tile_data;
} tile_set_t;

typedef struct {
  tile_set_t *tile_set;
  tile_t *data;
  tile_t border_tile;
  int width;
  int height;
} map_t;

tile_t map_get(const map_t *map, int x, int y);
bool map_solid(const map_t *map, int x, int y);
bool map_collide(const map_t *map, vec2_t pos, vec2_t box);

tile_data_t *tile_get_data(tile_set_t *tile_set, tile_t tile);
vec2_t tile_get_uv(const tile_set_t *tile_set, tile_t tile);

#endif
