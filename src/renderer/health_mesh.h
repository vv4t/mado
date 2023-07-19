#ifndef HEALTH_MESH_H
#define HEALTH_MESH_H

#include "mesh.h"
#include "camera.h"
#include "../game/game.h"

bool health_mesh_init(mesh_t *health_mesh, buffer_t *buffer);
void health_mesh_draw(mesh_t *health_mesh, const game_t *game, const camera_t *camera);

#endif
