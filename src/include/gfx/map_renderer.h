#ifndef MAP_RENDERER_H
#define MAP_RENDERER_H

#include <lib/map.h>

void map_renderer_init();
void map_renderer_draw();
void map_renderer_load(map_t map);
void map_renderer_deinit();

#endif
