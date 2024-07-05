#include <game/shoot.h>
#include <stdio.h>

void shoot_wall(game_t *gs, const shooter_t *sh, float ttl, vector o, vector fwd, float side, flight_t fl, float a1, float a2, int wall_bits, int length, float sep)
{
  vector v = normalize(mdotv(rotate_z(-M_PI / 2), fwd));
  vector start = vaddv(o, fdotv(-(length - 1) / 2.0, fdotv(sep, v)));
  for (int i = 0; i < length; i++) {
    if ((wall_bits >> i) & 1) {
      shoot(gs, sh, ttl, vaddv(start, fdotv((float)i, fdotv(sep, v))), fwd, side, fl, a1, a2);      
    }
  }
}

void shoot_radial(game_t *gs, const shooter_t *sh, float ttl, vector o, vector fwd, float side, flight_t fl, float a1, float a2, int count)
{
  for (int i = 0; i < count; i++) {
    float rot = i * (2 * M_PI / count);
    vector u = mdotv(rotate_z(rot), fwd);
    shoot(gs, sh, ttl, o, u, side, fl, a1, a2);
  }
}

void shoot_shotgun(game_t *gs, const shooter_t *sh, float ttl, vector o, vector fwd, float side, flight_t fl, float a1, float a2, int count, float cone)
{
  vector v = mdotv(rotate_z(-cone / 2), fwd);
  
  for (int i = 0; i < count; i++) {
    float rot = i * (cone / (count - 1));
    vector u = mdotv(rotate_z(rot), v);
    shoot(gs, sh, ttl, o, u, side, fl, a1, a2);
  }
}

entity_t shoot_linear(game_t *gs, const shooter_t *sh, float ttl, vector o, vector fwd)
{
  return shoot(gs, sh, ttl, o, fwd, 1.0, flight_linear, 0.0, 0.0);
}

entity_t shoot_wave(game_t *gs, const shooter_t *sh, float ttl, vector o, vector fwd, float amp, float freq, float phase)
{
  return shoot(gs, sh, ttl, o, fwd, amp, flight_wave, freq, phase);
}

entity_t shoot(game_t *gs, const shooter_t *sh, float ttl, vector o, vector fwd, float side, flight_t fl, float a1, float a2)
{
  entity_t e = entity_add(gs, ENT_BULLET);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.4;
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
    b->target = sh->target;
    b->damage = sh->damage;
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_do(a, ACT0, ttl);
  struct bulletctx *bulletctx = entity_get_context(gs, e, sizeof(struct bulletctx));
    bulletctx->a1 = a1;
    bulletctx->a2 = a2;
  entity_bind(gs, e, bullet_invoke);
  return e;
}

void bullet_invoke(game_t *gs, entity_t e, event_t ev)
{
  if (!entity_match(gs, e, C_bullet)) {
    return;
  }
  
  bullet_t *b = entity_get_component(gs, e, bullet);
  
  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      entity_kill(gs, e);
      break;
    }
    break;
  case EV_MAP_COLLIDE:
    entity_kill(gs, e);
    break;
  case EV_ENTITY_COLLIDE:
    if (b->target == entity_get_name(gs, ev.col.e)) {
      entity_invoke(gs, ev.col.e, (event_t) { .type = EV_HIT, .col.e = e });
      entity_kill(gs, e);
    }
    break;
  case EV_HIT:
  case EV_NO_HEALTH:
    break;
  }
}

vector flight_linear(float time, float a1, float a2)
{
  return vec2(0.0, 1.0);
}

vector flight_wave(float time, float a1, float a2)
{
  return vec4(cos(time * a1 + a2), 1.0, cos(time * a1 + a2), 1.0);
}

vector flight_accelerate(float time, float accel, float a2)
{
  return vec4(0.0, time * accel, 0.0, time * accel);
}
