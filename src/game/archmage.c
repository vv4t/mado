#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <stdio.h>

static const animation_t archmage_idle   = { .tx = 8, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.5 };
static const animation_t archmage_attack = { .tx = 8, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.4 };

static shooter_t archmage_shooter = {
  .tx = 2, .ty = 0,
  .tw = 1, .th = 1,
  .ttl = 5.0,
  .target = ENT_PLAYER
};

static void archmage_invoke(game_t *gs, entity_t e, event_t ev);

enum {
  ARCHMAGE_ACT_ATTACK = 1,
  ARCHMAGE_ACT_SHOOT
};

entity_t enemy_spawn_archmage(game_t *gs)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.0, 2.0, 2.0);
    t->position = vec2(5, 5);
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &archmage_idle);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 1.0;
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_set(a, 0, 10.0, 0);
    actor_set(a, 1, 2.0, 1);
    actor_set(a, 2, 4.0, 1);
    actor_set(a, 3, 6.0, 1);
    actor_start(a, 0);
    a->action[0].time = 2.0;
  entity_bind(gs, e, archmage_invoke);
  return e;
}

vector flight_archmage_bullet(float time, float radius, float speed) {
  float t1 = radius / speed;
  
  if (time < t1) {
    return vec2(0, -1);
  }
  
  time -= t1;
  
  float r = speed / radius;
  return vec2(cos(time * r), sin(time * r));
}

float a1 = 0.0;
float a2 = 1.0;

void archmage_invoke(game_t *gs, entity_t e, event_t ev)
{
  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  transform_t *t = entity_get_component(gs, e, transform);
  sprite_t *s = entity_get_component(gs, e, sprite);
  actor_t *a = entity_get_component(gs, e, actor);
  
  switch (ev.type) {
  case EV_ACT0:
    a1 = 0.0;
    actor_redo(a, 1);
    actor_redo(a, 2);
    actor_redo(a, 3);
    break;
  case EV_ACT1:
  case EV_ACT2:
  case EV_ACT3:
    sprite_play(s, &archmage_attack);
    actor_do(a, ARCHMAGE_ACT_ATTACK, 0.3, 1);
    a1 += 1.0;
    a2 = -a2;
    break;
  case EV_ACT:
    switch (ev.act.name) {
    case ARCHMAGE_ACT_ATTACK:
      actor_do(a, ARCHMAGE_ACT_SHOOT, 0.125, 12);
      break;
    case ARCHMAGE_ACT_SHOOT:
      shoot(gs, &archmage_shooter, t->position, vec2(0, 12), a2, flight_archmage_bullet, 2.0 + a1, 12.0);
      break;
    }
  }
}
