#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <lib/log.h>

#include <stdio.h>


static const animation_t mr_shadow_idle   = { .tx = 12, .ty = 10, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };
static const animation_t mr_shadow_attack = { .tx = 12, .ty = 8, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };

static shooter_t mr_shadow_shooter = {
  .tx = 3, .ty = 0,
  .tw = 1, .th = 1,
  .target = ENT_PLAYER,
  .damage = 20
};

static void mr_shadow_invoke(game_t *gs, entity_t e, event_t ev);
vector mr_shadow_movement(game_t *gs, entity_t e, float speed, float a2, float a3, float a4);

void enemy_spawn_mr_shadow(game_t *gs, vector spawn_pos)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(3.0, 3.0, 3.0);
    t->position = spawn_pos;
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_shadow_idle);
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.8;
  entity_add_component(gs, e, health);
    health_t *h = entity_get_component(gs, e, health);
    h->hp = 1000;
    h->max_hp = 1000;
  entity_add_component(gs, e, npcmove);
  entity_add_component(gs, e, automaton);
    automaton_t *st = entity_get_component(gs, e, automaton);
    automaton_add_transition(st, STATE0, STATE1, cond_lesser_hp_percent(0.5));
  entity_bind(gs, e, mr_shadow_invoke);
}

void mr_shadow_invoke(game_t *gs, entity_t e, event_t ev)
{
  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  transform_t *t = entity_get_component(gs, e, transform);
  sprite_t *s = entity_get_component(gs, e, sprite);
  actor_t *a = entity_get_component(gs, e, actor);
  npcmove_t *bm = entity_get_component(gs, e, npcmove);
  automaton_t *st = entity_get_component(gs, e, automaton);
  
  float pdist = length(vsubv(pt->position, t->position));
  
  vector forward = fdotv(3.0, normalize(vsubv(pt->position, t->position)));
  
  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      break;
    case ACT1:
      automaton_lock(st);
      sprite_play(s, &mr_shadow_attack);
      actor_do(a, ACT3, 0.5);
      actor_do(a, ACT4, 1.0);
      break;
    case ACT2:
      shoot_radial(gs, &mr_shadow_shooter, 5.0, t->position, forward, 1.0, flight_linear, 0.0, 0.0, 15);
      break;
    case ACT3:
      npcmove_chase(bm, 10.0);
      break;
    case ACT4:
      shoot_shotgun(gs, &mr_shadow_shooter, 5.0, t->position, forward, 1.0, flight_linear, 0.0, 0.0, 10, M_PI);
      automaton_unlock(st);
      npcmove_stop(bm);
      break;
    }
    break;
  case EV_TRANSITION:
    switch (ev.transition.state) {
    case STATE0:
      actor_stop_all(a);
      npcmove_stop(bm);
      actor_repeat(a, ACT1, 0.0, 0, 1.5);
      break;
    case STATE1:
      actor_stop_all(a);
      npcmove_stop(bm);
      actor_repeat(a, ACT2, 0.0, 0, 1.0);
      break;
    }
    break;
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
    break;
  }
}
