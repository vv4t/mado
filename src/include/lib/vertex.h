#ifndef VERTEX_H
#define VERTEX_H

#include <lib/math3d.h>

typedef struct {
  float p[3];
  float uv[2];
} vertex_t;

inline static vertex_t vertex_create(vector p, vector uv)
{
  return (vertex_t) {
    .p = { p.x, p.y, p.z },
    .uv = { uv.x, uv.y }
  };
}

#endif
