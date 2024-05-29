#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <stdio.h>

static const animation_t mr_dummy_idle = { .tx = 4, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };

static shooter_t mr_dummy_shooter = {
  .tx = 0, .ty = 0,
  .tw = 1, .th = 1,
  .ttl = 1.0,
  .target = ENT_PLAYER,
  .damage = 20
};

static void mr_dummy_invoke(game_t *gs, entity_t e, event_t ev);

void test_shoot(game_t *gs, vector o, vector forward);
void test_invoke(game_t *gs, entity_t e, event_t ev);

entity_t enemy_spawn_mr_dummy(game_t *gs)
{
  entity_t e = entity_add(gs, ENT_ANY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.0, 2.0, 2.0);
    t->position = vec2(6, 4);
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_dummy_idle);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.8;
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_do(a, ACT0, 2.0);
  entity_add_component(gs, e, health);
    health_t *h = entity_get_component(gs, e, health);
    h->hp = 100;
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
      test_shoot(gs, t->position, vec2(0, 3));
      break;
    }
    break;
  case EV_HIT:
    bullet_t *b = entity_get_component(gs, ev.entcol.e, bullet);
    health_t *h = entity_get_component(gs, e, health);
    h->hp -= b->damage;
    break;
  case EV_NO_HEALTH:
    entity_kill(gs, e);
    break;
  }
}

void test_shoot(game_t *gs, vector o, vector forward)
{
  entity_t b = shoot_linear(gs, &mr_dummy_shooter, o, forward);
  actor_t *ba = entity_get_component(gs, b, actor);
  actor_do(ba, ACT1, 0.95);
  entity_bind(gs, b, test_invoke);
}

void test_invoke(game_t *gs, entity_t e, event_t ev)
{
  transform_t *t = entity_get_component(gs, e, transform);
  rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
  
  vector once = mdotv(rotate_z(-M_PI / 6.0), rb->velocity);
  vector twice = mdotv(rotate_z(+M_PI / 6.0), rb->velocity);
  
  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT1:
      test_shoot(gs, t->position, once);
      test_shoot(gs, t->position, twice);
      break;
    }
    break;
  }
  
  bullet_invoke(gs, e, ev);
}
