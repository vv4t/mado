#include "mesh.h"

#include "../common/log.h"

void buffer_init(buffer_t *buffer, int max_vertices)
{
  glGenBuffers(1, &buffer->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, buffer-> vbo);
  glBufferData(GL_ARRAY_BUFFER, max_vertices * sizeof(vertex_t), 0, GL_DYNAMIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (float*) 0);
  
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (float*) 0 + 3);
  
  buffer->offset = 0;
  buffer->max_vertices = max_vertices;
}

bool buffer_new_mesh(
  buffer_t        *buffer,
  mesh_t          *mesh,
  const vertex_t  *vertices,
  int             num_vertices)
{
  if (buffer->offset + num_vertices > buffer->max_vertices) {
    LOG_ERROR("ran out of memory");
    return false;
  }
  
  mesh->offset = buffer->offset;
  mesh->count = num_vertices;
  buffer->offset += num_vertices;
  
  glBufferSubData(
    GL_ARRAY_BUFFER,
    mesh->offset * sizeof(vertex_t),
    num_vertices * sizeof(vertex_t),
    vertices
  );
  
  return true;
}


bool mesh_sub_data(
  mesh_t          *mesh,
  const vertex_t  *vertices,
  int             offset,
  int             num_vertices)
{
  if (offset + num_vertices > mesh->count) {
    LOG_ERROR("out of bounds");
    return false;
  }
  
  glBufferSubData(
    GL_ARRAY_BUFFER,
    (mesh->offset + offset) * sizeof(vertex_t),
    num_vertices * sizeof(vertex_t),
    vertices
  );
  
  return true;
}
