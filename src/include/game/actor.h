#ifndef ACTOR_H
#define ACTOR_H

#include <stdint.h>

#define ACTION_MAX 8
#define ACT_STATIC 255

typedef struct {
  uint8_t name;
  uint8_t active;
  uint8_t count;
  uint8_t max_count;
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

// fix actions to an id
inline static void actor_set(actor_t *a, int id, float time, int count)
{
  if (id >= ACTION_MAX) {
    return;
  }
  
  a->action[id] = (action_t) {
    .name = ACT_STATIC,
    .active = 0,
    .count = count,
    .max_count = count,
    .time = time,
    .max_time = time
  };
}

// do a temporary action which invokes EV_ACT with act.name = name
inline static int actor_do(actor_t *a, int name, float time, int count)
{
  for (int i = 0; i < ACTION_MAX; i++) {
    if (a->action[i].name == 0) {
      a->action[i] = (action_t) {
        .name = name,
        .active = 1,
        .count = count,
        .max_count = count,
        .time = time,
        .max_time = time
      };
      
      return i;
    }
  }
}

inline static void actor_start(actor_t *a, int id)
{
  if (id >= ACTION_MAX) {
    return;
  }
  
  a->action[id].active = 1;
}

inline static void actor_stop(actor_t *a, int id)
{
  if (id >= ACTION_MAX) {
    return;
  }
  
  a->action[id].active = 0;
}

inline static void actor_redo(actor_t *a, int id)
{
  if (id >= ACTION_MAX) {
    return;
  }
  
  a->action[id].count = a->action[id].max_count;
  a->action[id].time = a->action[id].max_time;
  a->action[id].active = 1;
}

#endif
