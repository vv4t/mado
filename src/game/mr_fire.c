#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <stdio.h>

static const animation_t mr_fire_idle   = { .tx = 8, .ty = 1, .tw = 1, .th = 1, .framecount = 3, .frametime = 0.125 };

static void mr_fire_invoke(game_t *gs, entity_t e, event_t ev);
vector mr_fire_movement(game_t *gs, entity_t e, float speed, float a2, float a3, float a4);

entity_t enemy_spawn_mr_fire(game_t *gs, vector spawn_pos)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(1.0, 1.0, 1.0);
    t->position = spawn_pos;
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_fire_idle);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.8;
  entity_add_component(gs, e, health);
    health_t *h = entity_get_component(gs, e, health);
    h->hp = 100;
    h->max_hp = 100;
  entity_bind(gs, e, mr_fire_invoke);
  return e;
}

void mr_fire_invoke(game_t *gs, entity_t e, event_t ev)
{
  switch (ev.type) {
  case EV_HIT:
    bullet_t *b = entity_get_component(gs, ev.col.e, bullet);
    health_t *h = entity_get_component(gs, e, health);
    h->hp -= b->damage;
    break;
  case EV_NO_HEALTH:
    entity_kill(gs, e);
    break;
  case EV_ENTITY_COLLIDE:
  case EV_MAP_COLLIDE:
  case EV_ACT:
    break;
  }
}
