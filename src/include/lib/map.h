#ifndef MAP_H
#define MAP_H

#include <lib/math3d.h>

typedef struct map_s *map_t;

map_t map_load(const char *path);
int map_get_width(map_t m);
int map_get_height(map_t m);
int map_get(map_t m, int x, int y);
int map_collide(map_t m, vector p, vector d);
void map_destroy(map_t m);

#endif
