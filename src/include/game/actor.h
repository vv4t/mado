#ifndef ACTOR_H
#define ACTOR_H

#include <stdint.h>

#define ACTION_MAX 8

typedef enum {
  ACT0, ACT1, ACT2, ACT3, ACT4, ACT5, ACT6, ACT7
} actname_t;

typedef struct {
  uint8_t active;
  uint8_t count;
  float   time;
  float   max_time;
} action_t;

typedef struct {
  action_t action[ACTION_MAX];
} actor_t;

inline static actor_t create_actor()
{
  return (actor_t) {0};
}

inline static void actor_do(actor_t *a, actname_t id, float time)
{
  if (id >= ACTION_MAX) {
    return;
  }
  
  a->action[id] = (action_t) {
    .active = 1,
    .count = 1,
    .time = time,
    .max_time = time
  };
}

inline static void actor_repeat(actor_t *a, actname_t id, float after, int count, float time)
{
  if (id >= ACTION_MAX) {
    return;
  }
  
  a->action[id] = (action_t) {
    .active = 1,
    .count = count,
    .time = after,
    .max_time = time
  };
}

inline static void actor_start(actor_t *a, actname_t id)
{
  if (id >= ACTION_MAX) {
    return;
  }
  
  a->action[id].active = 1;
}

inline static void actor_stop(actor_t *a, actname_t id)
{
  if (id >= ACTION_MAX) {
    return;
  }
  
  a->action[id].active = 0;
}

#endif
