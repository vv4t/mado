#ifndef C_TRANSFORM_H
#define C_TRANSFORM_H

#include "component.h"

typedef struct {
  vec2_t position;
  float rotation;
} c_transform_t;

inline component_t c_transform_init(c_transform_t *c_transform, vec2_t position, float rotation)
{
  c_transform->position = position;
  c_transform->rotation = rotation;
  return COMPONENT_TRANSFORM;
}

#endif
