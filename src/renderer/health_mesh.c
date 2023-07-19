#include "health_mesh.h"

#include "../common/log.h"
#include <string.h>

static const vertex_t health_vertices[] = {
  { .pos = { +0.5, +0.5, 1.0 }, .uv = { 1.0f, 0.0f } },
  { .pos = { +0.5, -0.5, 1.0 }, .uv = { 1.0f, 1.0f } },
  { .pos = { -0.5, +0.5, 1.0 }, .uv = { 0.0f, 0.0f } },
  { .pos = { -0.5, -0.5, 1.0 }, .uv = { 0.0f, 1.0f } },
  { .pos = { -0.5, +0.5, 1.0 }, .uv = { 0.0f, 0.0f } },
  { .pos = { +0.5, -0.5, 1.0 }, .uv = { 1.0f, 1.0f } }
};

const int num_health_vertices = sizeof(health_vertices) / sizeof(vertex_t);

#define MAX_HEALTH_VERTICES MAX_ENTITIES * num_health_vertices
  
bool health_mesh_init(mesh_t *mesh, buffer_t *buffer)
{
  if (!buffer_new_mesh(
    buffer,
    mesh,
    NULL,
    MAX_HEALTH_VERTICES
  )) {
    LOG_ERROR("failed to initialise mesh");
    return false;
  }
}

void health_mesh_draw(mesh_t *health_mesh, const game_t *game, const camera_t *camera)
{
  vertex_t vertices[MAX_HEALTH_VERTICES];
  memset(vertices, 0, sizeof(vertices));
  
  const component_t mask = COMPONENT_TRANSFORM | COMPONENT_HEALTH;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((game->edict.field[i] & mask) != mask)
      continue;
    
    if (game->cdict.health[i].health <= 0)
      continue;
    
    float interp_health = game->cdict.health[i].health / 100.0;
    
    vec3_t pos = vec3_init(game->cdict.transform[i].position.x, game->cdict.transform[i].position.y, 0.0);
    quat_t rot = camera->rot;
    vec3_t scale = vec3_init(interp_health, 0.0625, 1.0);
    
    mat4x4_t rot_mat = mat4x4_init_rotation(rot);
    mat4x4_t pos_mat = mat4x4_init_translation(pos);
    mat4x4_t scale_mat = mat4x4_init_scale(scale);
    mat4x4_t vert_mat = mat4x4_mul(mat4x4_mul(scale_mat, rot_mat), pos_mat);
    
    for (int j = 0; j < num_health_vertices; j++) {
      vec3_t pos = mat4x4_mul_vec3(vert_mat, health_vertices[j].pos);
      vec2_t uv = vec2_add(health_vertices[j].uv, vec2_init(3,0));
      
      vertices[i * num_health_vertices + j].pos = pos;
      vertices[i * num_health_vertices + j].uv = uv;
    }
  }
  
  mesh_sub_data(health_mesh, vertices, 0, MAX_HEALTH_VERTICES);
  
  glDrawArrays(GL_TRIANGLES, health_mesh->offset, MAX_HEALTH_VERTICES);
}
