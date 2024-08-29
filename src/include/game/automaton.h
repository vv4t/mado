#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <stdbool.h>
#include <lib/math3d.h>

typedef int entity_t;
typedef struct game_s game_t;

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

typedef union {
  struct { entity_t other; float trigger_dist; } distance;
  struct { float trigger_hp; } health;
  struct { float delay; } time;
} condition_args_t;

typedef bool (*test_t)(game_t *gs, entity_t e, condition_args_t args);

typedef struct condition_s {
  test_t test;
  condition_args_t args;
} condition_t;

typedef struct {
  state_t     from;
  state_t     to;
  condition_t condition;
  bool        active;
} transition_t;

typedef struct {
  state_t       current_state;
  transition_t  transitions[TRANSITION_MAX];
  float         last_transition_time;
  bool          locked;
} automaton_t;

inline static void automaton_add_transition(automaton_t *st, state_t from, state_t to, condition_t cond) {
  for (int i = 0; i < TRANSITION_MAX; i++) {
    if (!st->transitions[i].active) {
      st->transitions[i] = (transition_t) {
        .from = from,
        .to = to,
        .condition = cond,
        .active = true
      };
      break;
    }
  }
}

inline static automaton_t create_automaton()
{
  automaton_t st = (automaton_t) {
    .current_state = NOSTATE,
    .transitions = {{0}},
    .last_transition_time = 0,
    .locked = false,
  };
  automaton_add_transition(&st, NOSTATE, STATE0, (condition_t) { 0 });
  return st;
}

inline static void automaton_lock(automaton_t *st)
{
  st->locked = true;
}

inline static void automaton_unlock(automaton_t *st)
{
  st->locked = false;
}

condition_t cond_greater_distance(entity_t other, float dist);
condition_t cond_lesser_distance(entity_t other, float dist);
condition_t cond_lesser_hp_flat(float hp);
condition_t cond_lesser_hp_percent(float hp);
condition_t cond_time_elapsed(float delay);

#endif
