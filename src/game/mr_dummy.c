#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <stdio.h>

void mr_dummy_invoke(game_t *gs, entity_t e, event_t ev);

entity_t enemy_spawn_mr_dummy(game_t *gs)
{
  entity_t e = entity_add(gs, ENT_ANY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->position = vec3(3, 4, 0);
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    s->tx = 0;
    s->ty = 2;
    s->tw = 1;
    s->th = 1;
    s->orient = 1;
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_repeat(a, ACT0, 0.125, 0, 0.125);
  entity_bind(gs, e, mr_dummy_invoke);
  return e;
}

void mr_dummy_invoke(game_t *gs, entity_t e, event_t ev)
{
  transform_t *t = entity_get_component(gs, e, transform);
  
  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      t->position.z  = -1 - sin(gs->time);
      break;
    }
    break;
  case EV_HIT:
  case EV_NO_HEALTH:
  case EV_ENTITY_COLLIDE:
  case EV_MAP_COLLIDE:
    break;
  }
}
