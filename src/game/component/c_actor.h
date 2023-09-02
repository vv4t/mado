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
} action_t;

typedef struct {
  action_t action[MAX_ACTION];
} c_actor_t;

inline component_t c_actor_init(c_actor_t *c_actor)
{
  *c_actor = (c_actor_t) {0};
  return COMPONENT_ACTOR;
}

inline int c_actor_start(c_actor_t *c_actor, xaction_t xaction, float cooldown, int count)
{
  for (int i = 0; i < MAX_ACTION; i++) {
    if (c_actor->action[i].xaction) {
      continue;
    }
    
    c_actor->action[i].xaction = xaction;
    c_actor->action[i].cooldown = cooldown;
    c_actor->action[i].time = cooldown;
    c_actor->action[i].active = true;
    c_actor->action[i].count = count;
    
    return i;
  }
  
  return -1;
}

inline void c_actor_stop(c_actor_t *c_actor, int action)
{
  c_actor->action[action].xaction = NULL;
}

#endif
