#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <lib/log.h>

#include <stdio.h>


static const animation_t mr_mage_idle   = { .tx = 8, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.3 };
static const animation_t mr_mage_attack = { .tx = 8, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.2 };

static shooter_t mr_mage_shooter = {
  .tx = 2, .ty = 3,
  .tw = 1, .th = 1,
  .target = ENT_PLAYER,
  .damage = 20
};

static void mr_mage_invoke(game_t *gs, entity_t e, event_t ev);
vector mr_mage_movement(game_t *gs, entity_t e, float speed, float a2, float a3, float a4);

void enemy_spawn_mr_mage(game_t *gs, vector spawn_pos)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.0, 2.0, 2.0);
    t->position = spawn_pos;
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_mage_idle);
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_repeat(a, ACT0, 0.0, 0, 2.0);
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
  entity_bind(gs, e, mr_mage_invoke);
}

void mr_mage_invoke(game_t *gs, entity_t e, event_t ev)
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
      sprite_play(s, &mr_mage_attack);
      actor_do(a, ACT1, 0.3);
      break;
    case ACT1:
      for (int i = -4; i <= 4; i++) {
        float theta = i / 4.0 * M_PI;
        vector p = vaddv(forward, mdotv(rotate_z(M_PI - theta), fdotv(0.25, forward)));
        shoot_wave(gs, &mr_mage_shooter, 3.0, t->position, p, 2.0, 8.0, 0.0);
      }
      break;
    default:
      break;
    }
    break;
  case EV_TRANSITION:
    switch (ev.transition.state) {
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
