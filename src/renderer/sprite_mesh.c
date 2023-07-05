#include "sprite_mesh.h"

#include "../common/log.h"
#include <string.h>

static const vertex_t sprite_vertices[] = {
  { .pos = { +0.5, +0.5, 2.0 }, .uv = { 1.0f, 0.0f } },
  { .pos = { +0.5, -0.5, 0.0 }, .uv = { 1.0f, 1.0f } },
  { .pos = { -0.5, +0.5, 2.0 }, .uv = { 0.0f, 0.0f } },
  { .pos = { -0.5, -0.5, 0.0 }, .uv = { 0.0f, 1.0f } },
  { .pos = { -0.5, +0.5, 2.0 }, .uv = { 0.0f, 0.0f } },
  { .pos = { +0.5, -0.5, 0.0 }, .uv = { 1.0f, 1.0f } }
};

const int num_sprite_vertices = sizeof(sprite_vertices) / sizeof(vertex_t);

#define MAX_VERTICES MAX_ENTITIES * num_sprite_vertices
  
bool sprite_mesh_init(mesh_t *mesh, buffer_t *buffer)
{
  if (!buffer_new_mesh(
    buffer,
    mesh,
    NULL,
    MAX_VERTICES
  )) {
    LOG_ERROR("failed to initialise mesh");
    return false;
  }
}

void sprite_mesh_draw(mesh_t *sprite_mesh, const game_t *game, const camera_t *camera)
{
  vertex_t vertices[MAX_VERTICES];
  memset(vertices, 0, sizeof(vertices));
  
  const component_t mask = COMPONENT_TRANSFORM | COMPONENT_SPRITE;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((game->edict.field[i] & mask) != mask)
      continue;
    
    vec3_t pos = vec3_init(game->edict.transform[i].position.x, game->edict.transform[i].position.y, 0.0);
    quat_t rot = quat_init_rotation(vec3_init(0.0, 0.0, 1.0), game->edict.sprite[i].rotation);
    
    if (game->edict.sprite[i].orient) {
      rot = quat_mul(rot, camera->rot);
    }
    
    mat4x4_t stand_mat = mat4x4_init_translation(vec3_init(0, game->edict.sprite[i].stand * 0.5f, 0));
    mat4x4_t rot_mat = mat4x4_init_rotation(rot);
    mat4x4_t pos_mat = mat4x4_init_translation(pos);
    mat4x4_t vert_mat = mat4x4_mul(mat4x4_mul(stand_mat, rot_mat), pos_mat);
    
    for (int j = 0; j < num_sprite_vertices; j++) {
      vec3_t pos = mat4x4_mul_vec3(vert_mat, sprite_vertices[j].pos);
      vec2_t uv = vec2_add(sprite_vertices[j].uv, game->edict.sprite[i].uv);
      
      if (!game->edict.sprite[i].stand) {
        pos.z = 0.5;
      }
      
      vertices[i * num_sprite_vertices + j].pos = pos;
      vertices[i * num_sprite_vertices + j].uv = uv;
    }
  }
  
  mesh_sub_data(sprite_mesh, vertices, 0, MAX_VERTICES);
  
  glDrawArrays(GL_TRIANGLES, sprite_mesh->offset, MAX_VERTICES);
}
