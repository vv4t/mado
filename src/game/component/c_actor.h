#ifndef C_ACTOR_H
#define C_ACTOR_H

#define MAX_ACTION 4

#include "component.h"
#include "../game.h"
#include <stdbool.h>

typedef struct game_s game_t;

typedef struct action_s action_t;

typedef void (*xaction_t)(entity_t entity, action_t *action, game_t *game);

typedef struct action_s {
  xaction_t xaction;
  float time;
  float cooldown;
  int count;
  bool active;
  bool use;
} action_t;

typedef struct {
  action_t action[MAX_ACTION];
} c_actor_t;

inline component_t c_actor_init(c_actor_t *c_actor)
{
  *c_actor = (c_actor_t) {0};
  return COMPONENT_ACTOR;
}

inline void c_actor_set_act(c_actor_t *c_actor, int act, xaction_t xaction, float cooldown)
{
  if (act >= MAX_ACTION)
    return;
  
  c_actor->action[act].xaction = xaction;
  c_actor->action[act].cooldown = cooldown;
  c_actor->action[act].time = cooldown;
  c_actor->action[act].count = 0;
  c_actor->action[act].active = true;
  c_actor->action[act].use = true;
}

inline void c_actor_add_act(c_actor_t *c_actor, xaction_t xaction, float cooldown)
{
  for (int i = 0; i < MAX_ACTION; i++) {
    if (c_actor->action[i].use)
      continue;
    
    c_actor->action[i].xaction = xaction;
    c_actor->action[i].cooldown = cooldown;
    c_actor->action[i].time = 0.0;
    c_actor->action[i].active = true;
    c_actor->action[i].count = 0;
    c_actor->action[i].use = true;
    
    return;
  }
}

inline void c_actor_remove_act(c_actor_t *c_actor, action_t *action)
{
  action->use = false;
}

#endif
