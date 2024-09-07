#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <lib/log.h>

#include <stdio.h>

static const animation_t mr_reaper_idle   = { .tx = 12, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.3 };
static const animation_t mr_reaper_attack = { .tx = 12, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.2 };

static shooter_t mr_reaper_shooter = {
  .tx = 1, .ty = 3,
  .tw = 1, .th = 1,
  .target = ENT_PLAYER,
  .damage = 20
};

static shooter_t mr_reaper_shooter2 = {
  .tx = 3, .ty = 3,
  .tw = 1, .th = 1,
  .target = ENT_PLAYER,
  .damage = 20
};

static void mr_reaper_invoke(game_t *gs, entity_t e, event_t ev);
vector mr_reaper_movement(game_t *gs, entity_t e, float speed, float a2, float a3, float a4);

void enemy_spawn_mr_reaper(game_t *gs, vector spawn_pos)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.0, 2.0, 2.0);
    t->position = spawn_pos;
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_reaper_idle);
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
    automaton_add_transition(st, STATE1, STATE2, cond_lesser_distance(gs->player, 6.0));
    automaton_add_transition(st, STATE2, STATE0, cond_time_elapsed(0.9));
  entity_bind(gs, e, mr_reaper_invoke);
}

void mr_reaper_invoke(game_t *gs, entity_t e, event_t ev)
{
  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  transform_t *t = entity_get_component(gs, e, transform);
  sprite_t *s = entity_get_component(gs, e, sprite);
  actor_t *a = entity_get_component(gs, e, actor);
  health_t *h = entity_get_component(gs, e, health);
  npcmove_t *bm = entity_get_component(gs, e, npcmove);
  
  vector fwd1 = fdotv(8.0, normalize(vsubv(pt->position, t->position)));
  vector fwd4 = fdotv(12.0, normalize(vsubv(pt->position, t->position)));
  
  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      sprite_play(s, &mr_reaper_attack);
      actor_do(a, ACT1, 0.3);
      break;
    case ACT1:
      for (int j = 0; j <= 16; j++) {
        float theta = j / 16.0;
        vector fwd2 = mdotv(rotate_z(-theta * 2.8 * M_PI - 0.25), fwd1);
        vector fwd3 = fdotv(0.5 + theta * 0.9, fwd2);
        shoot_spin(gs, &mr_reaper_shooter, 3.0, t->position, fwd3, 20.0);
      }
      break;
    case ACT2:
      int wall = 0b11111;
      shoot_wall(gs, &mr_reaper_shooter2, 3.0, t->position, fwd4, 0.0, flight_linear, 0.0, 0.0, wall, 5.0, 2.25);
      break;
    case ACT3:
      sprite_play(s, &mr_reaper_attack);
      actor_do(a, ACT2, 0.3);
      break;
    case ACT4:
      actor_repeat(a, ACT3, 0.0, 3, 0.3);
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
      actor_repeat(a, ACT0, 0.0, 0, 2.0);
      break;
    case STATE1:
      npcmove_chase(bm, 16.0);
      npcmove_orbit(bm, 5.0);
      break;
    case STATE2:
      npcmove_chase(bm, 14.0);
      actor_do(a, ACT3, 0.0);
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
