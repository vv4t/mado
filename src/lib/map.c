#include <lib/map.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <lib/log.h>

struct map_s {
  tilename_t *data;
  bool *solid;
  int width;
  int height;
};

struct { tilename_t tilename; tile_t tile; } tiledata[] = {
  {
    .tilename = 0,
    .tile = { .tx = 0, .ty = 0, .solid = 1, .num_block = 0 }
  },
  {
    .tilename = 130,
    .tile = {
      .tx = 1, .ty = 7, .solid = 1, .num_block = 1,
      .block = { { .tx = 1, .ty = 7 } }
    }
  },
  {
    .tilename = 131,
    .tile = {
      .tx = 1, .ty = 7, .solid = 1, .num_block = 2,
      .block = { { .tx = 1, .ty = 7 },  { .tx = 2, .ty = 7 } }
    }
  },
  {
    .tilename = 132,
    .tile = {
      .tx = 3, .ty = 7, .solid = 1, .num_block = 1,
      .block = { { .tx = 3, .ty = 7 } }
    }
  },
};

map_t map_load(const char *path)
{
  FILE *fp = fopen(path, "r");
  if (!fp) {
    perror(path);
    exit(1);
  }
  
  map_t m = malloc(sizeof(*m));
  
  if (fscanf(fp, "%i %i", &m->width, &m->height) < 0) {
    LOG_ERROR("%s: bad header", path);
  }
  
  m->data = calloc(m->width * m->height, sizeof(tilename_t));
  m->solid = calloc(m->width * m->height, sizeof(bool));
  
  for (int i = 0; i < m->height; i++) {
    for (int j = 0; j < m->width; j++) {
      if (fscanf(fp, "%x", &m->data[i * m->width + j]) < 0) {
        LOG_ERROR("%s: bad data", path);
      }
      
      tilename_t tilename = m->data[i * m->width + j];
      tile_t tile;
      tile_get(&tile, tilename);
      
      if (tile.solid) {
        m->solid[i * m->width + j] = true;
      }
    }
  }
  
  return m;
}

int map_get_width(map_t m)
{
  return m->width;
}

int map_get_height(map_t m)
{
  return m->height;
}

int map_solid(map_t m, int x, int y)
{
  if (x < 0 || y < 0 || x >= m->width || y >= m->height) {
    return true;
  }
  
  return m->solid[x + y * m->width];
}

tilename_t map_get(map_t m, int x, int y)
{
  if (x < 0 || y < 0 || x >= m->width || y >= m->height) {
    return 0;
  }
  
  return m->data[x + y * m->width];
}

int map_collide(map_t m, vector p, vector d)
{
  int x0 = (int) floor(p.x - d.x);
  int y0 = (int) floor(p.y - d.y);
  int x1 = (int) floor(p.x + d.x);
  int y1 = (int) floor(p.y + d.y);
  
  if (map_solid(m, x0, y0) > 0) return 1;
  if (map_solid(m, x1, y0) > 0) return 1;
  if (map_solid(m, x0, y1) > 0) return 1;
  if (map_solid(m, x1, y1) > 0) return 1;
  
  return 0;
}

void map_destroy(map_t m)
{
  free(m->data);
  free(m);
}

void tile_get(tile_t *tile, tilename_t tilename)
{
  for (int i = 0; i < sizeof(tiledata) / sizeof(tiledata[0]); i++) {
    if (tiledata[i].tilename == tilename) {
      *tile = tiledata[i].tile;
      return;
    }
  }
  
  *tile = (tile_t) {
    .tx = (tilename - 1) % 16,
    .ty = 15 - (tilename - 1) / 16,
    .solid = false,
    .num_block = 0
  };
}
