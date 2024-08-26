#ifndef MAP_H
#define MAP_H

#include <lib/math3d.h>

typedef struct {
  int tx;
  int ty;
  int solid;
  int num_block;
  struct { int tx; int ty; } block[4];
} tile_t;

typedef int tilename_t;

typedef struct {
  char *name;
  vector position;
} landmark_t;

typedef struct map_s *map_t;

map_t map_load(const char *path);
int map_get_width(map_t m);
int map_get_height(map_t m);
tilename_t map_get(const map_t m, int x, int y);
int map_solid(const map_t m, int x, int y);
int map_collide(map_t m, vector p, vector d);
void map_destroy(map_t m);
vector map_landmark(map_t m, char *name);

void tile_get(tile_t *tile, tilename_t tilename);

#endif
