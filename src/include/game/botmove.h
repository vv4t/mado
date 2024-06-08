#ifndef BOTMOVE_H
#define BOTMOVE_H

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
} botmove_t;

inline static botmove_t create_botmove()
{
  return (botmove_t) {
    .behave = BH_STOP,
    .target = vec2(0.0, 0.0),
    .speed = 1.0,
    .time = 0.0
  };
}

inline static void botmove_stop(botmove_t *bm)
{
  bm->behave = BH_STOP;
}

inline static void botmove_chase(botmove_t *bm, float speed)
{
  bm->behave = BH_CHASE;
  bm->speed = speed;
}

inline static void botmove_retreat(botmove_t *bm, float speed)
{
  bm->behave = BH_RETREAT;
  bm->speed = speed;
}

inline static void botmove_travel(botmove_t *bm, vector target, float speed)
{
  bm->behave = BH_TRAVEL;
  bm->target = target;
  bm->speed = speed;
}

inline static void botmove_orbit(botmove_t *bm, vector target, float speed)
{
  bm->behave = BH_ORBIT;
  bm->time = 0.0;
  bm->target = target; 
  bm->speed = speed;
}

#endif
