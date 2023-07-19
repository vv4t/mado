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
  GLuint ul_size_uv;
  
  GLuint sprite_sheet;
  
  mesh_t map_mesh;
  mesh_t sprite_mesh;
  mesh_t health_mesh;
} renderer_t;

bool renderer_init(renderer_t *renderer);
void renderer_load_sheet(renderer_t *renderer, const sprite_sheet_t *sprite_sheet);
void renderer_load_map(renderer_t *renderer, const map_t *map);
void renderer_render(renderer_t *renderer, const game_t *game);

#endif
