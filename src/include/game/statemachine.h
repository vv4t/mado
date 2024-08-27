#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <lib/math3d.h>
#include <game/game.h>

typedef int entity_t;

#define TRANSITION_MAX 8

typedef enum {
  NOSTATE,
  STATE0,
  STATE1,
  STATE2,
  STATE3,
  STATE4,
  STATE5,
  STATE6,
  STATE7,
} state_t;

typedef bool (*condition_t)(struct game_s *gs, int e, int arg1, float arg2);

typedef struct {
  state_t     from;
  state_t     to;
  condition_t condition;
  entity_t    arg1;
  float       arg2;
  bool        active;
} transition_t;

typedef struct {
  state_t      currentState;
  transition_t transitions[TRANSITION_MAX];
  float        lastTransitionTime;
} statemachine_t;

inline static void statemachine_add_transition(statemachine_t *st, state_t from, state_t to, condition_t cond, entity_t arg1, float arg2) {
  for (int i = 0; i < TRANSITION_MAX; i++) {
    if (!st->transitions[i].active) {
      st->transitions[i] = (transition_t) {
        .from = from,
        .to = to,
        .condition = cond,
        .arg1 = arg1,
        .arg2 = arg2,
        .active = true
      };
      break;
    }
  }
}

inline static statemachine_t create_statemachine()
{
  statemachine_t st = (statemachine_t) {
    .currentState = NOSTATE,
    .transitions = {{0}},
    .lastTransitionTime = 0,
  };
  statemachine_add_transition(&st, NOSTATE, STATE0, NULL, 0, 0.0);
  return st;
}

#endif
