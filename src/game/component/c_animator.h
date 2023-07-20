#ifndef C_ANIMATOR_H
#define C_ANIMATOR_H

#include "component.h"
#include "../sprite.h"

typedef struct {
  const animation_t *animation;
  float time;
} c_animator_t;

inline component_t c_animator_init(c_animator_t *c_animator)
{
  c_animator->animation = NULL;
  c_animator->time = 0.0;
  return COMPONENT_ANIMATOR;
}

inline void c_animator_play(c_animator_t *animator, const animation_t *animation)
{
  if (animator->animation != animation) {
    animator->animation = animation;
    animator->time = 0.0;
  }
}

#endif
