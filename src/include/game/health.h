#ifndef HEALTH_H
#define HEALTH_H

#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int hp;
  int max_hp;
  bool invincible;
} health_t;

inline static health_t create_health()
{
  return (health_t) {
    .hp = 100,
    .max_hp = 100,
    .invincible = false
  };
}

inline static void health_damage(health_t *h, int damage) {
  if (h->invincible) {
    printf("invulnerable\n");
    return;
  } else {
    printf("vulnerable\n");
  }
  
  h->hp -= damage;

  if (h->hp > h->max_hp) {
    h->hp = h->max_hp;
  }

  if (h->hp < 0) {
    h->hp = 0;
  }
}

#endif
