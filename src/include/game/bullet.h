#ifndef BULLET_H
#define BULLET_H

#include <game/entdata.h>
#include <lib/math3d.h>

struct bulletctx {
  float a1, a2;
};

typedef vector (*flight_t)(float time, float a1, float a2);

typedef struct {
  flight_t  flight;
  vector    forward;
  vector    side;
  float     time;
  entname_t target;
} bullet_t;

inline static bullet_t create_bullet()
{
  return (bullet_t) {
    .flight   = NULL,
    .forward  = vec2(0.0, 1.0),
    .side     = vec2(1.0, 0.0),
    .time     = 0.0,
    .target   = ENT_ANY
  };
}

#endif
