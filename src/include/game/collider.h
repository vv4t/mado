#ifndef COLLIDER_H
#define COLLIDER_H

#include <game/bullet.h>

typedef struct {
  float radius;
  target_t type;
} collider_t;

inline static collider_t create_collider()
{
  return (collider_t) {
    .radius = 0.0,
    .type = TARGET_NONE
  };
}

#endif
