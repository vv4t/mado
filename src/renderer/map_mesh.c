#include "map_mesh.h"

#include "../common/log.h"

bool map_mesh_init(mesh_t *mesh, buffer_t *buffer)
{
  vertex_t vertices[] = {
    { .pos = {  0.0f,  0.0f, 4.0f }, .uv = { 0.0f, 0.0f } },
    { .pos = {  0.0f, +1.0f, 4.0f }, .uv = { 0.0f, 1.0f } },
    { .pos = { +1.0f,  0.0f, 4.0f }, .uv = { 1.0f, 0.0f } },
    { .pos = {  0.0f, +1.0f, 4.0f }, .uv = { 0.0f, 1.0f } },
    { .pos = { +1.0f, +1.0f, 4.0f }, .uv = { 1.0f, 1.0f } },
    { .pos = { +1.0f,  0.0f, 4.0f }, .uv = { 1.0f, 0.0f } }
  };
  
  if (!buffer_new_mesh(
    buffer,
    mesh,
    vertices,
    6
  )) {
    LOG_ERROR("failed to initialise mesh");
    return false;
  }
}
