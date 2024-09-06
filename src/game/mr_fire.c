#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <lib/log.h>

#include <stdio.h>

// static const animation_t mr_fire_idle   = { .tx = 4, .ty = 10, .tw = 1, .th = 1, .framecount = 1, .frametime = 0.50 };
static const animation_t mr_fire_attack = { .tx = 8, .ty = 0, .tw = 1, .th = 1, .framecount = 3, .frametime = 0.125 };

static shooter_t mr_fire_shooter = {
  .tx = 11, .ty = 0,
  .tw = 1, .th = 1,
  .target = ENT_PLAYER,
  .damage = 5
};

static void mr_fire_invoke(game_t *gs, entity_t e, event_t ev);
vector mr_fire_movement(game_t *gs, entity_t e, float speed, float a2, float a3, float a4);

void enemy_spawn_mr_fire(game_t *gs, vector spawn_pos)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(1.0, 1.0, 1.0);
    t->position = spawn_pos;
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_fire_attack);
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_do(a, ACT3, (rand() % 256) / 256.0);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.8;
  entity_add_component(gs, e, health);
    health_t *h = entity_get_component(gs, e, health);
    h->hp = 300;
    h->max_hp = 300;
  entity_add_component(gs, e, npcmove);
    npcmove_t *bm = entity_get_component(gs, e, npcmove);
    npcmove_stop(bm);
  entity_bind(gs, e, mr_fire_invoke);
}

void mr_fire_invoke(game_t *gs, entity_t e, event_t ev)
{
  transform_t *t = entity_get_component(gs, e, transform);
  health_t *h = entity_get_component(gs, e, health);
  actor_t *a = entity_get_component(gs, e, actor);
  npcmove_t *bm = entity_get_component(gs, e, npcmove);

  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  vector forward = fdotv(8.0, normalize(vsubv(pt->position, t->position)));

  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      float theta = cos(a->action[ev.act.name].count * M_PI / 4.0) * 0.5;
      shoot_linear(gs, &mr_fire_shooter, 1.5, t->position, mdotv(rotate_z(theta), forward));
      break;
    case ACT1:
      bm->speed = 2.0;
      actor_repeat(a, ACT0, 0.0, 16, 0.075);
      actor_do(a, ACT2, 40 * 0.075);
      break;
    case ACT2:
      bm->speed = 5.0;
      break;
    case ACT3:
      npcmove_chase(bm, 5.0);
      actor_repeat(a, ACT1, (rand() % 256) / 256.0 * 3, 0, 4.0);
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
