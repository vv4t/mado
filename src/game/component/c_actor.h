#ifndef C_ACTOR_H
#define C_ACTOR_H

#define MAX_ACTION 4

#include "component.h"
#include "../game.h"
#include <stdbool.h>

typedef struct game_s game_t;

typedef void (*xaction_t)(entity_t entity, game_t *game);

typedef struct {
  xaction_t xaction;
  float time;
  float cooldown;
  int count;
  bool active;
} action_t;

typedef struct {
  action_t action[MAX_ACTION];
} c_actor_t;

inline component_t c_actor_init(c_actor_t *c_actor)
{
  for (int i = 0; i < MAX_ACTION; i++) {
    c_actor->action[i] = (action_t) {0};
  }
  
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
}

#endif
