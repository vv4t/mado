#ifndef RENDERER_H
#define RENDERER_H

#include <game/game.h>
#include <lib/map.h>

void renderer_init();
void renderer_render(const game_t *game);
void renderer_load_map(map_t map);
void renderer_deinit();

#endif
