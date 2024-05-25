#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <stdio.h>

static const animation_t scytheman_idle   = { .tx = 12, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };
static const animation_t scytheman_attack = { .tx = 12, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.20 };

static shooter_t scytheman_shooter = {
  .tx = 3, .ty = 0,
  .tw = 1, .th = 1,
  .ttl = 1.0,
  .target = ENT_PLAYER
};

static void scytheman_invoke(game_t *gs, entity_t e, event_t ev);

entity_t enemy_spawn_scytheman(game_t *gs)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.0, 2.0, 2.0);
    t->position = vec2(6, 4);
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &scytheman_idle);
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_repeat(a, ACT0, 2.0, 0, 4.0);
    actor_start(a, 0);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.8;
  entity_bind(gs, e, scytheman_invoke);
  return e;
}

void scytheman_invoke(game_t *gs, entity_t e, event_t ev)
{
  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  transform_t *t = entity_get_component(gs, e, transform);
  sprite_t *s = entity_get_component(gs, e, sprite);
  actor_t *a = entity_get_component(gs, e, actor);
  
  vector forward = fdotv(3.0, normalize(vsubv(pt->position, t->position)));

  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      sprite_play(s, &scytheman_attack);
      actor_do(a, ACT1, 0.2);
      break;
    case ACT1:
      shoot_wall(gs, &scytheman_shooter, t->position, forward, 0b1011101, 7, 1.0);
      break;
    }
    break;
  }
}
