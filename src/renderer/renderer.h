#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"
#include "mesh.h"
#include "camera.h"
#include "../game/game.h"

typedef struct {
  buffer_t buffer;
  camera_t camera;
  
  GLuint program;
  GLuint ul_mvp;
  
  GLuint texture;
  
  mat4x4_t proj_mat;
  
  mesh_t map_mesh;
  mesh_t sprite_mesh;
} renderer_t;

bool renderer_init(renderer_t *renderer);
void renderer_load_map(renderer_t *renderer, const map_t *map);
void renderer_render(renderer_t *renderer, const game_t *game);

#endif
