#ifndef C_BULLET_H
#define C_BULELT_H

#include "component.h"

typedef struct {
  c_tag_t target;
  int damage;
} c_bullet_t;

inline component_t c_bullet_init(c_bullet_t *c_bullet, c_tag_t target, int damage)
{
  c_bullet->target = target;
  c_bullet->damage = damage;
  return COMPONENT_BULLET;
}

#endif
