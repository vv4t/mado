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

typedef struct map_s *map_t;
typedef struct landmark_s *landmark_t;

map_t map_load(const char *path);
int map_get_width(map_t m);
int map_get_height(map_t m);
tile_t map_get(const map_t m, int x, int y);
int map_collide(map_t m, vector p, vector d);
landmark_t map_get_landmarks(map_t m);
void map_destroy(map_t m);

const char *landmark_get_name(landmark_t lm);
vector landmark_get_position(landmark_t lm);
landmark_t landmark_next(landmark_t lm);

#endif
