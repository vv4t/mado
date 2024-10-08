#ifndef MESH_H
#define MESH_H

#include <lib/math3d.h>
#include <lib/vertex.h>

typedef struct meshdata_s *meshdata_t;

meshdata_t meshdata_create();
void meshdata_add_vertex(meshdata_t md, vertex_t v);
void meshdata_add_face(meshdata_t md, vertex_t v1, vertex_t v2, vertex_t v3);
void meshdata_add_quad(meshdata_t md, matrix T_p, matrix T_uv);
const vertex_t *meshdata_get_vertices(meshdata_t md);
int meshdata_get_size(meshdata_t md);
void meshdata_destroy(meshdata_t md);

typedef struct {
  int offset;
  int count;
} mesh_t;

void vbuffer_init(int max_vertices);
mesh_t vbuffer_add(meshdata_t md);
void vbuffer_bind();
void vbuffer_draw(mesh_t mesh);
void vbuffer_deinit();

#endif
