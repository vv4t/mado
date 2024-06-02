#include <game/enemy.h>
#include <game/shoot.h>
#include <game/movement.h>
#include <game/game.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum {
  SCYTHEBOSS_PHASE_INACTIVE,
  SCYTHEBOSS_PHASE0,
  SCYTHEBOSS_PHASE1,
} scytheboss_phase_t;

static const animation_t mr_scytheboss_idle = { .tx = 12, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };
static const animation_t mr_scytheboss_attack = { .tx = 12, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.20 };

static shooter_t mr_scytheboss_shooter = {
  .tx = 2, .ty = 0,
  .tw = 1, .th = 1,
  .ttl = 5.0,
  .target = ENT_PLAYER,
  .damage = 20
};

struct scythebossctx {
  scytheboss_phase_t phase;
};

void mr_scytheboss_phase_change(game_t *gs, entity_t e);
void mr_scytheboss_phase0_invoke(game_t *gs, entity_t e, event_t ev);
void mr_scytheboss_phase1_invoke(game_t *gs, entity_t e, event_t ev);
void mr_scytheboss_phase_inactive_invoke(game_t *gs, entity_t e, event_t ev);
void mr_scytheboss_invoke(game_t *gs, entity_t e, event_t ev);

entity_t enemy_spawn_mr_scytheboss(game_t *gs)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.5, 2.5, 2.5);
    t->position = vec2(48/2 + 4, 4 * 48/6);
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_scytheboss_idle);
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
  entity_bind(gs, e, mr_scytheboss_invoke);

  struct scythebossctx *ctx = entity_get_context(gs, e, sizeof(struct scythebossctx));
  ctx->phase = SCYTHEBOSS_PHASE_INACTIVE;

  return e;
}

void mr_scytheboss_invoke(game_t *gs, entity_t e, event_t ev)
{
  sprite_t *s = entity_get_component(gs, e, sprite);
  actor_t *a = entity_get_component(gs, e, actor);
  health_t *h = entity_get_component(gs, e, health);
  
  struct scythebossctx *ctx = entity_get_context(gs, e, sizeof(struct scythebossctx));

  switch (ev.type) {
  case EV_ACT:
    switch(ctx->phase) {
    case SCYTHEBOSS_PHASE_INACTIVE:
      mr_scytheboss_phase_inactive_invoke(gs, e, ev);
      break;
    case SCYTHEBOSS_PHASE0:
      mr_scytheboss_phase0_invoke(gs, e, ev);
      break;
    case SCYTHEBOSS_PHASE1:
      mr_scytheboss_phase1_invoke(gs, e, ev);
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

void mr_scytheboss_phase_change(game_t *gs, entity_t e) {
  actor_t *a = entity_get_component(gs, e, actor);
  botmove_t *bm = entity_get_component(gs, e, botmove);

  struct scythebossctx *ctx = entity_get_context(gs, e, sizeof(struct scythebossctx));

  actor_stop_all(a);
  botmove_stop(bm);

  switch (ctx->phase) {
    case SCYTHEBOSS_PHASE0:
      ctx->phase = SCYTHEBOSS_PHASE1;
      break;
    case SCYTHEBOSS_PHASE1:
      ctx->phase = SCYTHEBOSS_PHASE0;
      break;
    case SCYTHEBOSS_PHASE_INACTIVE:
      break;
  }
  
  actor_do(a, ACT1, 0.0);
}

void mr_scytheboss_phase0_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);
  actor_t *a = entity_get_component(gs, e, actor);
  health_t *h = entity_get_component(gs, e, health);

  switch(ev.act.name) {
    case ACT0:
      mr_scytheboss_phase_change(gs, e);
      break;
    case ACT1:
      h->invincible = false;
      actor_do(a, ACT0, 10.0);
      actor_repeat(a, ACT2, 1.0, 0, 0.1);
      break;
    case ACT2:
      shoot_radial(
        gs, 
        &mr_scytheboss_shooter, 
        t->position, 
        vec2(0.0, 3.0), -1.0, 
        flight_linear, 0.0, 0.0, 
        2
      );
      break;
    case ACT3:
    case ACT4:
    case ACT5:
    case ACT6:
    case ACT7:
      break;
  }
}

void mr_scytheboss_phase1_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);
  actor_t *a = entity_get_component(gs, e, actor);
  health_t *h = entity_get_component(gs, e, health);

  switch(ev.act.name) {
    case ACT0:
      mr_scytheboss_phase_change(gs, e);
      break;
    case ACT1:
      h->invincible = false;
      actor_do(a, ACT0, 10.0);
      actor_repeat(a, ACT2, 1.0, 0, 0.1);
      break;
    case ACT2:
      shoot_radial(
        gs, 
        &mr_scytheboss_shooter, 
        t->position, 
        vec2(0.0, 3.0), -1.0, 
        flight_linear, 0.0, 0.0, 
        15
      );
      break;
    case ACT3:
    case ACT4:
    case ACT5:
    case ACT6:
    case ACT7:
      break;
  }
}

void mr_scytheboss_phase_inactive_invoke(game_t *gs, entity_t e, event_t ev) {
  health_t *h = entity_get_component(gs, e, health);
  actor_t *a = entity_get_component(gs, e, actor);
  sprite_t *s = entity_get_component(gs, e, sprite);

  struct scythebossctx *ctx = entity_get_context(gs, e, sizeof(struct scythebossctx));

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;
      ctx->phase = SCYTHEBOSS_PHASE1;
      actor_do(a, ACT1, 0.0);
      sprite_repeat(s, &mr_scytheboss_idle);
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
