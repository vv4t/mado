#ifndef renderer_sprite_H
#define renderer_sprite_H

#include <game/game.h>
#include <gfx/mesh.h>

void renderer_sprite_init(mesh_t mesh);
void renderer_sprite_draw(const game_t *gs);
void renderer_sprite_deinit();

#endif
