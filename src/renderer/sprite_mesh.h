#ifndef SPRITE_MESH_H
#define SPRITE_MESH_H

#include "mesh.h"
#include "camera.h"
#include "../game/game.h"

bool sprite_mesh_init(mesh_t *sprite_mesh, buffer_t *buffer);
void sprite_mesh_draw(mesh_t *sprite_mesh, const game_t *game, const camera_t *camera);

#endif
