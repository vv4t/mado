#ifndef HEALTH_H
#define HEALTH_H

typedef struct {
  int hp;
  int max_hp;
} health_t;

inline static health_t create_health()
{
  return (health_t) {
    .hp = 100,
    .max_hp = 100
  };
}

#endif
