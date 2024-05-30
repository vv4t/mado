#ifndef R_SPRITE_H
#define R_SPRITE_H

#include <game/game.h>
#include <renderer/mesh.h>

void r_sprite_init(mesh_t mesh);
void r_sprite_draw(const game_t *gs);
void r_sprite_deinit();

#endif
