#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <game/game.h>
#include <gfx/mesh.h>

void sprite_renderer_init(mesh_t mesh);
void sprite_renderer_draw(const game_t *gs);
void sprite_renderer_deinit();

#endif
