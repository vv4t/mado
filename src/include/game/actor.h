#ifndef ACTOR_H
#define ACTOR_H

typedef void (*act_t)(struct game_s *gs, int entity);

typedef struct {
  act_t act;
  float time;
  int count;
} action_t;

typedef struct {
  action_t action[MAX_ACTION];
} actor_t;

void actor_perform();

#endif
