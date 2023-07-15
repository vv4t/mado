#ifndef C_ACTOR_H
#define C_ACTOR_H

#define MAX_ACT 4

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
} act_t;

typedef struct {
  act_t act[MAX_ACT];
  int num_act;
  float angle;
} c_actor_t;

#endif
