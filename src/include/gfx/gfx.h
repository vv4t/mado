#ifndef gfx_H
#define gfx_H

#include <game/game.h>
#include <lib/map.h>

void gfx_init();
void gfx_render(const game_t *game);
void gfx_load_map(map_t map);
void gfx_deinit();

#endif
