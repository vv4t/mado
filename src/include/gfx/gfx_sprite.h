#ifndef gfx_sprite_H
#define gfx_sprite_H

#include <game/game.h>
#include <gfx/mesh.h>

void gfx_sprite_init(mesh_t mesh);
void gfx_sprite_draw(const game_t *gs);
void gfx_sprite_deinit();

#endif
