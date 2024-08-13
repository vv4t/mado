#ifndef renderer_H
#define renderer_H

#include <game/game.h>
#include <lib/map.h>

void renderer_init();
void renderer_render(const game_t *game);
void renderer_map_load(map_t map);
void renderer_deinit();

#endif
