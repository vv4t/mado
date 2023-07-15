#ifndef C_ANIMATOR_H
#define C_ANIMATOR_H

#include "../sprite.h"

typedef struct {
  const animation_t *animation;
  float time;
} c_animator_t;

inline void c_animator_play(c_animator_t *animator, const animation_t *animation)
{
  if (animator->animation != animation) {
    animator->animation = animation;
    animator->time = 0.0;
  }
}

#endif
