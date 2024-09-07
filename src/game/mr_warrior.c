#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <lib/log.h>

#include <stdio.h>


static const animation_t mr_warrior_idle   = { .tx = 4, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };
static const animation_t mr_warrior_attack = { .tx = 4, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.20 };
static const animation_t mr_warrior_fall = { .tx = 4, .ty = 8, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };

static shooter_t mr_warrior_shooter = {
  .tx = 3, .ty = 3,
  .tw = 1, .th = 1,
  .target = ENT_PLAYER,
  .damage = 20
};

static void mr_warrior_invoke(game_t *gs, entity_t e, event_t ev);
vector mr_warrior_movement(game_t *gs, entity_t e, float speed, float a2, float a3, float a4);

void enemy_spawn_mr_warrior(game_t *gs, vector spawn_pos)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.0, 2.0, 2.0);
    t->position = spawn_pos;
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_warrior_idle);
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.8;
  entity_add_component(gs, e, health);
    health_t *h = entity_get_component(gs, e, health);
    h->hp = 2000;
    h->max_hp = 2000;
  entity_add_component(gs, e, npcmove);
    npcmove_t *bm = entity_get_component(gs, e, npcmove);
    npcmove_chase(bm, 4.0);
    npcmove_orbit(bm, 4.0);
  entity_add_component(gs, e, automaton);
    automaton_t *st = entity_get_component(gs, e, automaton);
    automaton_add_transition(st, STATE0, STATE1, cond_time_elapsed(2.5));
    automaton_add_transition(st, STATE0, STATE3, cond_lesser_hp_flat(700));
    automaton_add_transition(st, STATE1, STATE2, cond_lesser_distance(gs->player, 5.0));
    automaton_add_transition(st, STATE2, STATE0, cond_time_elapsed(0.9));
  entity_bind(gs, e, mr_warrior_invoke);
}

void mr_warrior_invoke(game_t *gs, entity_t e, event_t ev)
{
  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  transform_t *t = entity_get_component(gs, e, transform);
  sprite_t *s = entity_get_component(gs, e, sprite);
  actor_t *a = entity_get_component(gs, e, actor);
  npcmove_t *bm = entity_get_component(gs, e, npcmove);
  health_t *h = entity_get_component(gs, e, health);
  
  vector forward = fdotv(8.0, normalize(vsubv(pt->position, t->position)));
  
  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      sprite_play(s, &mr_warrior_attack);
      actor_do(a, ACT1, 0.2);
      break;
    case ACT1:
      shoot_shotgun(gs, &mr_warrior_shooter, 1.0, t->position, forward, 1.0, flight_linear, 0.0, 0.0, 3, M_PI / 3);
      break;
    case ACT2:
      actor_repeat(a, ACT0, 0.0, 3, 0.3);
      break;
    case ACT4:
      shoot_radial(gs, &mr_warrior_shooter, 5.0, t->position, forward, 1.0, flight_linear, 0.0, 0.0, 15);
      break;
    default:
      break;
    }
    break;
  case EV_TRANSITION:
    switch (ev.transition.state) {
    case STATE0:
      npcmove_chase(bm, 4.0);
      npcmove_orbit(bm, 4.0);
      break;
    case STATE1:
      npcmove_chase(bm, 16.0);
      npcmove_orbit(bm, 3.0);
      break;
    case STATE2:
      npcmove_chase(bm, 14.0);
      actor_do(a, ACT2, 0.0);
      break;
    case STATE3:
      npcmove_stop(bm);
      actor_stop_all(a);
      sprite_repeat(s, &mr_warrior_fall);
      actor_repeat(a, ACT4, 0.0, 0, 1.0);
      break;
    default:
      break;
    }
    break;
  case EV_HIT:
    h->hp -= ev.hit.damage;
    break;
  case EV_NO_HEALTH:
    entity_kill(gs, e);
    break;
  default:
    break;
  }
}
