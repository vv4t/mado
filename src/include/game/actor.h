#ifndef ACTOR_H
#define ACTOR_H

#define ACTION_MAX 8

typedef struct {
  float time;
  int   count;
  float max_time;
  int   max_count;
  int   active;
} action_t;

typedef struct {
  action_t action[ACTION_MAX];
} actor_t;

inline static actor_t create_actor()
{
  return (actor_t) {0};
}

inline static void actor_set(actor_t *a, int id, float time, int count)
{
  if (id >= ACTION_MAX) {
    return;
  }
  
  a->action[id] = (action_t) {
    .time = time,
    .count = count,
    .max_time = time,
    .max_count = count,
    .active = 0
  };
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
