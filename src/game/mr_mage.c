#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <stdio.h>

static const animation_t mr_mage_idle   = { .tx = 8, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.5 };
static const animation_t mr_mage_attack = { .tx = 8, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.4 };

static shooter_t mr_mage_shooter = {
  .tx = 2, .ty = 0,
  .tw = 1, .th = 1,
  .ttl = 5.0,
  .target = ENT_PLAYER,
  .damage = 20
};

struct magectx {
  float orbit_radius;
  float orbit_direction;
  float shoot_angle;
};

static void mr_mage_invoke(game_t *gs, entity_t e, event_t ev);

entity_t enemy_spawn_mr_mage(game_t *gs)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.0, 2.0, 2.0);
    t->position = vec2(5, 5);
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_mage_idle);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 1.0;
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_repeat(a, ACT0, 2.0, 0, 14.0);
  entity_add_component(gs, e, health);
    health_t *h = entity_get_component(gs, e, health);
    h->hp = 100;
  entity_bind(gs, e, mr_mage_invoke);
  return e;
}

vector flight_mr_mage_bullet(float time, float radius, float speed) {
  float t1 = radius / speed;
  
  if (time < t1) {
    return vec2(0, -1);
  }
  
  time -= t1;
  
  float r = speed / radius;
  return vec2(cos(time * r), sin(time * r));
}

void mr_mage_invoke(game_t *gs, entity_t e, event_t ev)
{
  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  transform_t *t = entity_get_component(gs, e, transform);
  sprite_t *s = entity_get_component(gs, e, sprite);
  actor_t *a = entity_get_component(gs, e, actor);
  
  float speed = 12.0;
  
  struct magectx *magectx = entity_get_context(gs, e, sizeof(struct magectx));
  vector forward = mdotv(rotate_z(magectx->shoot_angle), vec2(0, speed));
  
  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      magectx->orbit_radius = 8.0;
      magectx->orbit_direction = 1.0;
      actor_do(a, ACT1, 2.0);
      actor_do(a, ACT2, 5.0);
      actor_do(a, ACT3, 8.0);
      break;
    case ACT1:
    case ACT2:
    case ACT3:
      sprite_play(s, &mr_mage_attack);
      actor_repeat(a, ACT4, 0.5, 12, 0.125);
      magectx->orbit_radius -= 1.5;
      magectx->orbit_direction *= -1.0;
      magectx->shoot_angle = (rand() % 256) / 256.0 * 2 * M_PI;
      break;
    case ACT4:
      shoot(
        gs, &mr_mage_shooter, t->position,
        forward, magectx->orbit_direction,
        flight_mr_mage_bullet,
        magectx->orbit_radius, speed
      );
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
