#include "map_mesh.h"

#include "../common/log.h"
#include <stdlib.h>

#define MAX_VERTICES 1024

int put_tile(vertex_t vertices[], int offset, int max_vertices, vec3_t pos);
int put_block(vertex_t vertices[], int offset, int max_vertices, vec3_t pos);
int put_quad(vertex_t vertices[], int offset, int max_vertices, mat4x4_t transform);

bool map_mesh_init(mesh_t *mesh, buffer_t *buffer)
{
  vertex_t vertices[MAX_VERTICES];
  
  int num_vertices = 0;
  
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 10; x++) {
      vec3_t pos = vec3_init(x, y, 0.0);
      
      if (rand() % 5 == 0) {
        num_vertices = put_block(vertices, num_vertices, MAX_VERTICES, pos);
        pos.z -= 1.0;
      }
      
      num_vertices = put_tile(vertices, num_vertices, MAX_VERTICES, pos);
    }
  }
  
  if (!buffer_new_mesh(
    buffer,
    mesh,
    vertices,
    num_vertices
  )) {
    LOG_ERROR("failed to initialise mesh");
    return false;
  }
}

int put_block(vertex_t vertices[], int offset, int max_vertices, vec3_t pos)
{
  mat4x4_t side1_mat = mat4x4_init(
    vec4_init(0.0, 0.0, 1.0, pos.x),
    vec4_init(0.0, 1.0, 0.0, pos.y),
    vec4_init(1.0, 0.0, 0.0, pos.z - 1.0),
    vec4_init(0.0, 0.0, 0.0, 1.0)
  );
  
  mat4x4_t side2_mat = mat4x4_init(
    vec4_init(0.0, 0.0, 1.0, pos.x + 1.0),
    vec4_init(0.0, 1.0, 0.0, pos.y),
    vec4_init(1.0, 0.0, 0.0, pos.z - 1.0),
    vec4_init(0.0, 0.0, 0.0, 1.0)
  );
  
  mat4x4_t front1_mat = mat4x4_init(
    vec4_init(0.0, 1.0, 0.0, pos.x),
    vec4_init(0.0, 0.0, 1.0, pos.y),
    vec4_init(1.0, 0.0, 0.0, pos.z - 1.0),
    vec4_init(0.0, 0.0, 0.0, 1.0)
  );
  
  mat4x4_t front2_mat = mat4x4_init(
    vec4_init(0.0, 1.0, 0.0, pos.x),
    vec4_init(0.0, 0.0, 1.0, pos.y + 1.0),
    vec4_init(1.0, 0.0, 0.0, pos.z - 1.0),
    vec4_init(0.0, 0.0, 0.0, 1.0)
  );
  
  offset = put_quad(vertices, offset, max_vertices, side1_mat);
  offset = put_quad(vertices, offset, max_vertices, side2_mat);
  offset = put_quad(vertices, offset, max_vertices, front1_mat);
  offset = put_quad(vertices, offset, max_vertices, front2_mat);
  
  return offset;
}

int put_tile(vertex_t vertices[], int offset, int max_vertices, vec3_t pos)
{
  mat4x4_t transform = mat4x4_init_translation(pos);
  
  return put_quad(
    vertices,
    offset,
    max_vertices,
    transform
  );
}

int put_quad(vertex_t vertices[], int offset, int max_vertices, mat4x4_t transform)
{
  if (offset + 6 > max_vertices)
    return 0;
  
  vertex_t quad_vertices[] = {
    { .pos = {  0.0f,  0.0f, 0.0f }, .uv = { 0.0f, 0.0f } },
    { .pos = {  0.0f, +1.0f, 0.0f }, .uv = { 0.0f, 1.0f } },
    { .pos = { +1.0f,  0.0f, 0.0f }, .uv = { 1.0f, 0.0f } },
    { .pos = {  0.0f, +1.0f, 0.0f }, .uv = { 0.0f, 1.0f } },
    { .pos = { +1.0f, +1.0f, 0.0f }, .uv = { 1.0f, 1.0f } },
    { .pos = { +1.0f,  0.0f, 0.0f }, .uv = { 1.0f, 0.0f } }
  };
  
  for (int i = 0; i < 6; i++) {
    vec3_t pos = mat4x4_mul_vec3(transform, quad_vertices[i].pos);
    
    vertices[offset + i].pos = pos;
    vertices[offset + i].uv = quad_vertices[i].uv;
  }
  
  return offset + 6;
}
