#ifndef C_HEALTH_H
#define C_HEALTH_H

#include "component.h"

typedef struct game_s game_t;

typedef void (*xdie_t)(entity_t entity, game_t *game);

typedef struct {
  int health;
  xdie_t xdie;
} c_health_t;

inline component_t c_health_init(c_health_t *c_health, int health)
{
  c_health->health = health;
  c_health->xdie = NULL;
  return COMPONENT_HEALTH;
}

inline void c_health_apply_damage(c_health_t *c_health, int damage)
{
  c_health->health -= damage;
}

#endif
