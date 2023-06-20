#include "map_mesh.h"

#include "../common/log.h"
#include <stdlib.h>

#define MAX_VERTICES 1024

typedef struct {
  vertex_t vertices[MAX_VERTICES];
  int offset;
} quad_buf_t;

void put_tile(quad_buf_t *quad_buf, vec3_t pos, vec2_t uv);
void put_block(quad_buf_t *quad_buf, vec3_t pos, vec2_t uv);
void put_quad(quad_buf_t *quad_buf, mat4x4_t transform, mat3x3_t uv_mat);

bool map_mesh_init(mesh_t *mesh, buffer_t *buffer)
{
  quad_buf_t quad_buf;
  quad_buf.offset = 0;
  
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 10; x++) {
      vec3_t pos = vec3_init(x, y, 0.0);
      
      if (rand() % 5 == 0) {
        put_block(&quad_buf, pos, vec2_init(1, 1));
        pos.z -= 1.0;
      }
      
      put_tile(&quad_buf, pos, vec2_init(0, 1));
    }
  }
  
  if (!buffer_new_mesh(
    buffer,
    mesh,
    quad_buf.vertices,
    quad_buf.offset
  )) {
    LOG_ERROR("failed to initialise mesh");
    return false;
  }
}

void put_block(quad_buf_t *quad_buf, vec3_t pos, vec2_t uv)
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
  
  mat3x3_t uv_mat = mat3x3_init(
    vec3_init(0.0, 1.0, 0.0 + uv.x),
    vec3_init(1.0, 0.0, 0.0 + uv.y),
    vec3_init(0.0, 0.0, 1.0)
  );
  
  put_quad(quad_buf, side1_mat, uv_mat);
  put_quad(quad_buf, side2_mat, uv_mat);
  put_quad(quad_buf, front1_mat, uv_mat);
  put_quad(quad_buf, front2_mat, uv_mat);
}

void put_tile(quad_buf_t *quad_buf, vec3_t pos, vec2_t uv)
{
  return put_quad(quad_buf, mat4x4_init_translation(pos), mat3x3_init_translation(uv));
}

void put_quad(quad_buf_t *quad_buf, mat4x4_t pos_mat, mat3x3_t uv_mat)
{
  if (quad_buf->offset + 6 > MAX_VERTICES)
    return;
  
  vertex_t quad[] = {
    { .pos = {  0.0f,  0.0f, 0.0f }, .uv = { 0.0f, 0.0f } },
    { .pos = {  0.0f, +1.0f, 0.0f }, .uv = { 0.0f, 1.0f } },
    { .pos = { +1.0f,  0.0f, 0.0f }, .uv = { 1.0f, 0.0f } },
    { .pos = {  0.0f, +1.0f, 0.0f }, .uv = { 0.0f, 1.0f } },
    { .pos = { +1.0f, +1.0f, 0.0f }, .uv = { 1.0f, 1.0f } },
    { .pos = { +1.0f,  0.0f, 0.0f }, .uv = { 1.0f, 0.0f } }
  };
  
  for (int i = 0; i < 6; i++) {
    vec3_t pos = mat4x4_mul_vec3(pos_mat, quad[i].pos);
    vec2_t uv = mat3x3_mul_vec2(uv_mat, quad[i].uv);
    
    quad_buf->vertices[quad_buf->offset + i].pos = pos;
    quad_buf->vertices[quad_buf->offset + i].uv = uv;
  }
  
  quad_buf->offset += 6;
}
