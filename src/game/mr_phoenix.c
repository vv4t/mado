#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <lib/log.h>

#include <stdio.h>

static const animation_t mr_phoenix_attack = { .tx = 8, .ty = 1, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.15 };

static shooter_t mr_phoenix_shooter = {
  .tx = 3, .ty = 0,
  .tw = 1, .th = 1,
  .target = ENT_PLAYER,
  .damage = 20
};

static void mr_phoenix_invoke(game_t *gs, entity_t e, event_t ev);
vector mr_phoenix_movement(game_t *gs, entity_t e, float speed, float a2, float a3, float a4);

void enemy_spawn_mr_phoenix(game_t *gs, vector spawn_pos)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.0, 2.0, 2.0);
    t->position = spawn_pos;
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_phoenix_attack);
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_do(a, ACT2, 0.5);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.8;
  entity_add_component(gs, e, health);
    health_t *h = entity_get_component(gs, e, health);
    h->hp = 1000;
    h->max_hp = 1000;
  entity_add_component(gs, e, npcmove);
    npcmove_t *bm = entity_get_component(gs, e, npcmove);
    npcmove_stop(bm);
  entity_bind(gs, e, mr_phoenix_invoke);
}

void mr_phoenix_invoke(game_t *gs, entity_t e, event_t ev)
{
  transform_t *t = entity_get_component(gs, e, transform);
  health_t *h = entity_get_component(gs, e, health);
  actor_t *a = entity_get_component(gs, e, actor);
  npcmove_t *bm = entity_get_component(gs, e, npcmove);

  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  vector forward = fdotv(16.0, normalize(vsubv(pt->position, t->position)));

  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      shoot_shotgun(gs, &mr_phoenix_shooter, 5.0, t->position, forward, 1.0, flight_linear, 0.0, 0.0, 5, M_PI / 7.0);
      break;
    case ACT1:
      vector target = vaddv(pt->position, mdotv(rotate_z(gs->time * 7.0 / 5.0), vec2(0, 5)));
      npcmove_travel(bm, target, 7.0);
      break;
    case ACT2:
      actor_repeat(a, ACT0, 1.0, 0, 2.0);
      actor_repeat(a, ACT1, 0.0, 0, 0.25);
      break;
    }
    break;
  case EV_TRANSITION:
    break;
  case EV_HIT:
    h->hp -= ev.hit.damage;
    break;
  case EV_NO_HEALTH:
    entity_kill(gs, e);
    break;
  case EV_ENTITY_COLLIDE:
  case EV_MAP_COLLIDE:
    break;
  }
}
