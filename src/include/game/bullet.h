#ifndef BULLET_H
#define BULLET_H

#include <lib/math3d.h>

typedef vector (*flight_t)(float time, float a1, float a2);

typedef enum {
  TARGET_NONE,
  TARGET_PLAYER,
  TARGET_ENEMY
} target_t;

typedef struct {
  flight_t  flight;
  vector    forward;
  vector    side;
  float     time;
  float     a1;
  float     a2;
  target_t  target;
} bullet_t;

inline static bullet_t create_bullet()
{
  return (bullet_t) {
    .flight   = NULL,
    .forward  = vec2(0.0, 1.0),
    .side     = vec2(1.0, 0.0),
    .time     = 0.0,
    .a1       = 0.0,
    .a2       = 0.0,
    .target   = TARGET_NONE
  };
}

#endif
