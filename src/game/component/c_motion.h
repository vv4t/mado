#ifndef C_MOTION_H
#define C_MOTION_H

#include "component.h"
#include "../../common/nui_math.h"

typedef struct {
  vec2_t velocity;
  float rotation;
  vec2_t new_pos;
} c_motion_t;

inline component_t c_motion_init(c_motion_t *c_motion)
{
  c_motion->velocity = vec2_init(0,0);
  c_motion->rotation = 0.0;
  c_motion->new_pos = vec2_init(0,0);
  return COMPONENT_MOTION;
}

#endif
