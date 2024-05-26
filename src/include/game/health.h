#ifndef HEALTH_H
#define HEALTH_H

typedef struct {
  int hp;
} health_t;

inline static health_t create_health()
{
  return (health_t) {
    .hp = 100
  };
}

#endif
