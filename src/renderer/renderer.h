#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"
#include "mesh.h"
#include "camera.h"

typedef struct {
  buffer_t buffer;
  camera_t camera;
  
  GLuint program;
  GLuint ul_mvp;
  
  GLuint texture;
  
  mat4x4_t proj_mat;
  
  mesh_t mesh;
} renderer_t;

bool renderer_init(renderer_t *renderer);
void renderer_render(renderer_t *renderer);

#endif
