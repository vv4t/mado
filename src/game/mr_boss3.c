#include <game/enemy.h>
#include <game/shoot.h>
#include <game/movement.h>
#include <game/game.h>
#include <game/boss.h>
#include <stdio.h>
#include <stdbool.h>

static const animation_t mr_mageboss_idle = { .tx = 8, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };
static const animation_t mr_mageboss_attack = { .tx = 8, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.20 };

static shooter_t mr_mageboss_shooter = {
  .tx = 2, .ty = 0,
  .tw = 1, .th = 1,
  .ttl = 5.0,
  .target = ENT_PLAYER,
  .damage = 20
};

void mr_mageboss_phase0_invoke(game_t *gs, entity_t e, event_t ev);
void mr_mageboss_phase1_invoke(game_t *gs, entity_t e, event_t ev);
void mr_mageboss_phase_inactive_invoke(game_t *gs, entity_t e, event_t ev);
void mr_mageboss_invoke(game_t *gs, entity_t e, event_t ev);

entity_t enemy_spawn_mr_mageboss(game_t *gs, vector pos)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.5, 2.5, 2.5);
    t->position = pos;
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_mageboss_idle);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.8;
  entity_add_component(gs, e, actor);
  entity_add_component(gs, e, health);
    health_t *h = entity_get_component(gs, e, health);
    h->hp = 2000;
    h->max_hp = 2000;
    h->invincible = true;
  entity_add_component(gs, e, botmove);
    botmove_t *bm = entity_get_component(gs, e, botmove);
    botmove_stop(bm);
  entity_bind(gs, e, mr_mageboss_invoke);

  bossctx_t *ctx = entity_get_context(gs, e, sizeof(bossctx_t));
  ctx->phase = BOSS_PHASE_INACTIVE;

  return e;
}

void mr_mageboss_invoke(game_t *gs, entity_t e, event_t ev)
{
  health_t *h = entity_get_component(gs, e, health);
  
  bossctx_t *ctx = entity_get_context(gs, e, sizeof(bossctx_t));

  switch (ev.type) {
  case EV_ACT:
    switch(ctx->phase) {
    case BOSS_PHASE_INACTIVE:
      mr_mageboss_phase_inactive_invoke(gs, e, ev);
      break;
    case BOSS_PHASE0:
      mr_mageboss_phase0_invoke(gs, e, ev);
      break;
    case BOSS_PHASE1:
      mr_mageboss_phase1_invoke(gs, e, ev);
      break;
    }
    break;
  case EV_HIT:
    bullet_t *b = entity_get_component(gs, ev.entcol.e, bullet);
    health_damage(h, b->damage);
    break;
  case EV_NO_HEALTH:
    entity_kill(gs, e);
    break;
  case EV_MAP_COLLIDE:
    break;
  case EV_ENTITY_COLLIDE:
    break;
  }
}

void mr_mageboss_phase0_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);
  actor_t *a = entity_get_component(gs, e, actor);
  sprite_t *s = entity_get_component(gs, e, sprite);
  health_t *h = entity_get_component(gs, e, health);
  botmove_t *bm = entity_get_component(gs, e, botmove);

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;
      actor_stop_all(a);
      botmove_travel(bm, vec2(24.0, 24.0), 10.0);
      actor_repeat(a, ACT1, 0.0, 0, 2.0);
      actor_repeat(a, ACT2, 1.0, 0, 2.0);
      break;
    case ACT1:
      if (t->position.x == 24.0 && t->position.y == 24.0) {
        sprite_play(s, &mr_mageboss_attack);
        vector v = vec2(0.0, 3.0);
        shoot_radial(
          gs, 
          &mr_mageboss_shooter, 
          t->position, 
          v, 1.0, 
          flight_linear, 0.0, 0.0, 
          10
        );
      }
      break;
    case ACT2:
      if (t->position.x == 24.0 && t->position.y == 24.0) {
        sprite_play(s, &mr_mageboss_attack);
        vector v = mdotv(rotate_z(M_PI / 10), vec2(0.0, 3.0));
        shoot_radial(
          gs, 
          &mr_mageboss_shooter, 
          t->position, 
          v, 1.0, 
          flight_linear, 0.0, 0.0, 
          10
        );
      }
      break;
    case ACT3:
    case ACT4:
    case ACT5:
    case ACT6:
    case ACT7:
      break;
  }
}

void mr_mageboss_phase1_invoke(game_t *gs, entity_t e, event_t ev) {
  actor_t *a = entity_get_component(gs, e, actor);
  health_t *h = entity_get_component(gs, e, health);
  botmove_t *bm = entity_get_component(gs, e, botmove);
  sprite_t *s = entity_get_component(gs, e, sprite);

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;
      botmove_stop(bm);
      actor_stop_all(a);
      actor_repeat(a, ACT1, 1.0, 0, 0.5);
      break;
    case ACT1:
      sprite_play(s, &mr_mageboss_attack);
      break;
    case ACT2:
    case ACT3:
    case ACT4:
    case ACT5:
    case ACT6:
    case ACT7:
      break;
  }
}

void mr_mageboss_phase_inactive_invoke(game_t *gs, entity_t e, event_t ev) {
  health_t *h = entity_get_component(gs, e, health);
  actor_t *a = entity_get_component(gs, e, actor);
  sprite_t *s = entity_get_component(gs, e, sprite);

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;
      sprite_repeat(s, &mr_mageboss_idle);
      actor_stop_all(a);
      actor_do(a, ACT0, 0.0);
      break;
    case ACT1:
    case ACT2:
    case ACT3:
    case ACT4:
    case ACT5:
    case ACT6:
    case ACT7:
      break;
  }
}
