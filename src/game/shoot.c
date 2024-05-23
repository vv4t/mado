#include <game/shoot.h>
#include <stdio.h>

vector flight_linear(float time, float a1, float a2);
vector flight_wave(float time, float a1, float a2);
void bullet_invoke(game_t *gs, entity_t e, event_t ev);

entity_t shoot_linear(game_t *gs, const shooter_t *sh, vector o, vector fwd)
{
  shoot(gs, sh, o, fwd, 1.0, flight_linear, 0.0, 0.0);
}

entity_t shoot_wave(game_t *gs, const shooter_t *sh, vector o, vector fwd, float amp, float freq, float phase)
{
  shoot(gs, sh, o, fwd, amp, flight_wave, freq, phase);
}

entity_t shoot(game_t *gs, const shooter_t *sh, vector o, vector fwd, float side, flight_t fl, float a1, float a2)
{
  entity_t e = entity_add(gs);
  entity_add_component(gs, e, rigidbody);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->position = o;
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    s->tx = sh->tx;
    s->ty = sh->ty;
    s->tw = sh->tw;
    s->th = sh->th;
    s->orient = 0;
  entity_add_component(gs, e, bullet);
    bullet_t *b = entity_get_component(gs, e, bullet);
    b->flight = fl;
    b->forward = fwd;
    b->side = cross(b->forward, vec3(0, 0, side));
    b->a1 = a1;
    b->a2 = a2;
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_set(a, 0, sh->ttl, 1);
    actor_start(a, 0);
  entity_bind(gs, e, bullet_invoke);
  
  return e;
}

void bullet_invoke(game_t *gs, entity_t e, event_t ev)
{
  switch (ev.type) {
  case EV_ACT0:
    entity_kill(gs, e);
    break;
  case EV_MAP_COLLIDE:
    entity_kill(gs, e);
    break;
  }
}

vector flight_linear(float time, float a1, float a2) {
  return vec2(0.0, 1.0);
}

vector flight_wave(float time, float a1, float a2) {
  return vec2(cos(time * a1 + a2), 1.0);
}

