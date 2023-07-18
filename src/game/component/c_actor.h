#ifndef C_ACTOR_H
#define C_ACTOR_H

#define MAX_ACTION 4

#include <stdbool.h>

typedef int entity_t;
typedef struct edict_s edict_t;

typedef void (*xaction_t)(entity_t entity, edict_t *edict);

typedef struct {
  xaction_t xaction;
  float time;
  float cooldown;
  int count;
  bool active;
} action_t;

typedef struct {
  action_t action[MAX_ACTION];
  float angle;
} c_actor_t;

inline void c_actor_set_act(c_actor_t *c_actor, int act, xaction_t xaction, float cooldown)
{
  if (act >= MAX_ACTION)
    return;
  
  c_actor->action[act].xaction = xaction;
  c_actor->action[act].cooldown = cooldown;
  c_actor->action[act].time = cooldown;
  c_actor->action[act].count = 0;
  c_actor->action[act].active = true;
}

#endif
