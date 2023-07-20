#ifndef AI_MOVE_H
#define AI_MOVE_H

#include "component.h"

typedef struct {
  vec2_t pivot;
} c_ai_move_t;

inline component_t c_ai_move_init(c_ai_move_t *c_ai_move, vec2_t pivot)
{
  c_ai_move->pivot = pivot;
  return COMPONENT_AI_MOVE;
}

#endif
