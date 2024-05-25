#include <game/shoot.h>
#include <stdio.h>

vector flight_linear(float time, float a1, float a2);
vector flight_wave(float time, float a1, float a2);
void bullet_invoke(game_t *gs, entity_t e, event_t ev);

entity_t shoot_wall(game_t *gs, const shooter_t *sh, vector o, vector fwd, target_t target, int wall_bits, int length, float sep)
{
  vector v = {
    .x = fwd.x * cos(-M_PI / 2) - fwd.y * sin(-M_PI / 2),
    .y = fwd.x * sin(-M_PI / 2) + fwd.y * cos(-M_PI / 2)
  };
  v = normalize(v);

  vector start = vaddv(o, fdotv(-(length - 1) / 2.0, fdotv(sep, v)));

  for (int i = 0; i < length; i++) {
    if ((wall_bits >> i) & 1) {
      shoot(gs, sh, vaddv(start, fdotv((float)i, fdotv(sep, v))), fwd, target, 1.0, flight_linear, 0.0, 0.0);      
    }
  }
}

entity_t shoot_radial(game_t *gs, const shooter_t *sh, vector o, vector fwd, target_t target, int count)
{
  for (int i = 0; i < count; i++) {
    float rot = i * (2 * M_PI / count);

    vector u = {
      .x = fwd.x * cos(rot) - fwd.y * sin(rot),
      .y = fwd.x * sin(rot) + fwd.y * cos(rot)
    };

    shoot(gs, sh, o, u, target, 1.0, flight_linear, 0.0, 0.0);
  }
}

entity_t shoot_shotgun(game_t *gs, const shooter_t *sh, vector o, vector fwd, target_t target, int count, float cone)
{
  vector v = {
    .x = fwd.x * cos(-cone / 2) - fwd.y * sin(-cone / 2),
    .y = fwd.x * sin(-cone / 2) + fwd.y * cos(-cone / 2)
  };
  for (int i = 0; i < count; i++) {
    float rot = i * (cone / (count - 1));

    vector u = {
      .x = v.x * cos(rot) - v.y * sin(rot),
      .y = v.x * sin(rot) + v.y * cos(rot)
    };

    shoot(gs, sh, o, u, target, 1.0, flight_linear, 0.0, 0.0);
  }
}

entity_t shoot_linear(game_t *gs, const shooter_t *sh, vector o, vector fwd, target_t target)
{
  shoot(gs, sh, o, fwd, target, 1.0, flight_linear, 0.0, 0.0);
}

entity_t shoot_wave(game_t *gs, const shooter_t *sh, vector o, vector fwd, target_t target, float amp, float freq, float phase)
{
  shoot(gs, sh, o, fwd, target, amp, flight_wave, freq, phase);
}

entity_t shoot(game_t *gs, const shooter_t *sh, vector o, vector fwd, target_t target, float side, flight_t fl, float a1, float a2)
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
    b->target = target;
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
  case EV_ENTITY_COLLIDE:
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

