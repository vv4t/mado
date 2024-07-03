#ifndef BEHAVE_H
#define BEHAVE_H

#include <lib/math3d.h>

typedef int entity_t;

typedef enum {
  BH_STOP,
  BH_TRAVEL,
  BH_CHASE,
  BH_RETREAT,
  BH_ORBIT
} behave_t;

typedef struct {
  behave_t  behave;
  vector    target;
  float     speed;
  float     time;
} behave_t;

inline static behave_t create_behave()
{
  return (behave_t) {
    .behave = BH_STOP,
    .target = vec2(0.0, 0.0),
    .speed = 1.0,
    .time = 0.0
  };
}

inline static void behave_stop(behave_t *bm)
{
  bm->behave = BH_STOP;
}

inline static void behave_chase(behave_t *bm, float speed)
{
  bm->behave = BH_CHASE;
  bm->speed = speed;
}

inline static void behave_retreat(behave_t *bm, float speed)
{
  bm->behave = BH_RETREAT;
  bm->speed = speed;
}

inline static void behave_travel(behave_t *bm, vector target, float speed)
{
  bm->behave = BH_TRAVEL;
  bm->target = target;
  bm->speed = speed;
}

inline static void behave_orbit(behave_t *bm, vector target, float speed)
{
  bm->behave = BH_ORBIT;
  bm->time = 0.0;
  bm->target = target; 
  bm->speed = speed;
}

#endif
