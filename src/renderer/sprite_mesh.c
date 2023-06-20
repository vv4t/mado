#include "sprite_mesh.h"

#include "../common/log.h"

static const vertex_t sprite_vertices[] = {
  { .pos = { +0.5, +0.5, 1.25 }, .uv = { 1.0f, 0.0f } },
  { .pos = { +0.5, -0.5, 0.0 }, .uv = { 1.0f, 1.0f } },
  { .pos = { -0.5, +0.5, 1.25 }, .uv = { 0.0f, 0.0f } },
  { .pos = { -0.5, -0.5, 0.0 }, .uv = { 0.0f, 1.0f } },
  { .pos = { -0.5, +0.5, 1.25 }, .uv = { 0.0f, 0.0f } },
  { .pos = { +0.5, -0.5, 0.0 }, .uv = { 1.0f, 1.0f } }
};

static const int num_sprite_vertices = sizeof(sprite_vertices) / sizeof(vertex_t);
  
bool sprite_mesh_init(mesh_t *mesh, buffer_t *buffer)
{
  if (!buffer_new_mesh(
    buffer,
    mesh,
    NULL,
    MAX_SPRITES * num_sprite_vertices
  )) {
    LOG_ERROR("failed to initialise mesh");
    return false;
  }
}

void sprite_mesh_draw(mesh_t *sprite_mesh, const sprite_t sprites[MAX_SPRITES], const camera_t *camera)
{
  vertex_t vertices[MAX_SPRITES * num_sprite_vertices];
  
  for (int i = 0; i < MAX_SPRITES; i++) {
    if (!sprites[i].show)
      continue;
    
    vec3_t pos = vec3_init(sprites[i].pos.x, sprites[i].pos.y, 0.0);
    
    mat4x4_t stand_mat = mat4x4_init_translation(vec3_init(0, sprites[i].stand * 0.5f, 0));
    mat4x4_t rot_mat = mat4x4_init_rotation(camera->rot);
    mat4x4_t pos_mat = mat4x4_init_translation(pos);
    mat4x4_t vert_mat = mat4x4_mul(mat4x4_mul(stand_mat, rot_mat), pos_mat);
    
    for (int j = 0; j < num_sprite_vertices; j++) {
      vec3_t pos = mat4x4_mul_vec3(vert_mat, sprite_vertices[j].pos);
      vec2_t uv = vec2_add(sprite_vertices[j].uv, sprites[i].uv);
      
      vertices[i * num_sprite_vertices + j].pos = pos;
      vertices[i * num_sprite_vertices + j].uv = uv;
    }
  }
  
  mesh_sub_data(sprite_mesh, vertices, 0, 6);
  
  glDrawArrays(GL_TRIANGLES, sprite_mesh->offset, num_sprite_vertices);
}
