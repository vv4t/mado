#include <game/automaton.h>
#include <game/game.h>

bool test_greater_distance(game_t *gs, entity_t e, condition_args_t args) {
  const transform_t *st = entity_get_component(gs, e, transform);
  const transform_t *ot = entity_get_component(gs, args.distance.other, transform);
  float pdist = length(vsubv(st->position, ot->position));
  return pdist >= args.distance.trigger_dist;
}

condition_t cond_greater_distance(entity_t other, float dist) {
  return (condition_t) {
    .test = test_greater_distance,
    .args.distance.other = other,
    .args.distance.trigger_dist = dist
  };
}

bool test_lesser_distance(game_t *gs, entity_t e, condition_args_t args) {
  const transform_t *st = entity_get_component(gs, e, transform);
  const transform_t *ot = entity_get_component(gs, args.distance.other, transform);
  float pdist = length(vsubv(st->position, ot->position));
  return pdist < args.distance.trigger_dist;
}

condition_t cond_lesser_distance(entity_t other, float dist) {
  return (condition_t) {
    .test = test_lesser_distance,
    .args.distance.other = other,
    .args.distance.trigger_dist = dist
  };
}

bool test_lesser_hp_flat(game_t *gs, entity_t e, condition_args_t args) {
  const health_t *h = entity_get_component(gs, e, health);
  return h->hp < args.health.trigger_hp;
}

condition_t cond_lesser_hp_flat(float hp) {
  return (condition_t) {
    .test = test_lesser_hp_flat,
    .args.health.trigger_hp = hp
  };
}

bool test_lesser_hp_percent(game_t *gs, entity_t e, condition_args_t args) {
  const health_t *h = entity_get_component(gs, e, health);
  return h->hp < h->max_hp * args.health.trigger_hp;
}

condition_t cond_lesser_hp_percent(float hp) {
  return (condition_t) {
    .test = test_lesser_hp_percent,
    .args.health.trigger_hp = hp
  };
}

bool test_time_elapsed(game_t *gs, entity_t e, condition_args_t args) {
  const automaton_t *st = entity_get_component(gs, e, automaton);
  return gs->time - st->last_transition_time >= args.time.delay;
}

condition_t cond_time_elapsed(float delay) {
  return (condition_t) {
    .test = test_time_elapsed,
    .args.time.delay = delay
  };
}
