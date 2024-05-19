#ifndef ACTOR_H
#define ACTOR_H

#define ACTION_FIXED_MAX 4
#define ACTION_TEMP_MAX 4
#define ACTION_MAX (ACTION_FIXED_MAX + ACTION_TEMP_MAX)

struct game_s;

typedef void (*act_t)(struct game_s *gs, int e);

typedef struct {
  act_t act;
  int   count;
  float time;
  int   maxcount;
  float maxtime;
} action_t;

typedef struct {
  action_t action[ACTION_MAX];
} actor_t;

inline static actor_t create_actor()
{
  return (actor_t) {0};
}

inline static void actor_fix(actor_t *a, int id, act_t act, float time, int count)
{
  if (id >= ACTION_FIXED_MAX) {
    return;
  }
  
  a->action[id] = (action_t) {
    .act = act,
    .time = time,
    .count = count,
    .maxtime = time,
    .maxcount = count
  };
}

inline static void actor_do(actor_t *a, act_t act, float time, int count)
{
  for (int i = ACTION_FIXED_MAX; i < ACTION_MAX; i++) {
    if (a->action[i].act != NULL) {
      continue;
    }
    
    a->action[i] = (action_t) {
      .act = act,
      .time = time,
      .count = count,
      .maxtime = time,
      .maxcount = count
    };
    
    break;
  }
}

#endif
