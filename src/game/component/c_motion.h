#ifndef C_MOTION_H
#define C_MOTION_H

#include "../../common/nui_math.h"

typedef struct {
  vec2_t velocity;
  float rotation;
  vec2_t new_pos;
} c_motion_t;

#endif
