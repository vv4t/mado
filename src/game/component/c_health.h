#ifndef C_HEALTH_H
#define C_HEALTH_H

typedef struct game_s game_t;

typedef void (*xdie_t)(entity_t entity, game_t *game);

typedef struct {
  int health;
  xdie_t xdie;
} c_health_t;

inline void c_health_apply_damage(c_health_t *c_health, int damage)
{
  c_health->health -= damage;
}

#endif
