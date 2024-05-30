#include <game/enemy.h>
#include <game/shoot.h>
#include <game/movement.h>
#include <game/game.h>
#include <stdio.h>

typedef enum {
  SWORDBOSS_PHASE0,
  SWORDBOSS_PHASE1
} swordboss_phase_t;

static const animation_t mr_swordboss_idle = { .tx = 4, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };

static shooter_t mr_swordboss_shooter = {
  .tx = 3, .ty = 0,
  .tw = 1, .th = 1,
  .ttl = 1.0,
  .target = ENT_PLAYER,
  .damage = 20
};

static shooter_t mr_swordboss_shooter2 = {
  .tx = 3, .ty = 0,
  .tw = 1, .th = 1,
  .ttl = 5.0,
  .target = ENT_PLAYER,
  .damage = 20
};

struct swordbossctx {
  swordboss_phase_t phase;
  float             rot_offset;
};

void mr_swordboss_invoke(game_t *gs, entity_t e, event_t ev);

entity_t enemy_spawn_mr_swordboss(game_t *gs)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.0, 2.0, 2.0);
    t->position = vec2(6, 4);
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_swordboss_idle);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.8;
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_repeat(a, ACT0, 2.0, 0, 10.0);
    // play animation
  entity_add_component(gs, e, health);
    health_t *h = entity_get_component(gs, e, health);
    h->hp = 1000;
  entity_add_component(gs, e, botmove);
    botmove_t *m = entity_get_component(gs, e, botmove);
    m->movement = movement_none;
  entity_bind(gs, e, mr_swordboss_invoke);
  return e;
}

void mr_swordboss_invoke(game_t *gs, entity_t e, event_t ev)
{
  const transform_t *pt = entity_get_component(gs, gs->player, transform);

  transform_t *t = entity_get_component(gs, e, transform);
  actor_t *a = entity_get_component(gs, e, actor);
  botmove_t *m = entity_get_component(gs, e, botmove);
  health_t *h = entity_get_component(gs, e, health);
  
  struct swordbossctx *ctx = entity_get_context(gs, e, sizeof(struct swordbossctx));

  vector slow_fwd = fdotv(3.0, normalize(vsubv(pt->position, t->position)));
  vector medium_fwd = fdotv(8.0, normalize(vsubv(pt->position, t->position)));
  vector fast_fwd = fdotv(15.0, normalize(vsubv(pt->position, t->position)));

  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      // halt current phase
      ctx->rot_offset = 0.0;
      actor_stop(a, ACT2);
      actor_stop(a, ACT3);
      m->movement = movement_none;

      // switch phase
      switch (ctx->phase) {
        case SWORDBOSS_PHASE0:
          ctx->phase = SWORDBOSS_PHASE1;
          break;
        case SWORDBOSS_PHASE1:
          ctx->phase = SWORDBOSS_PHASE0;
          break;
      }
      actor_do(a, ACT1, 0.0);
      break;
    case ACT1:
      // phase start
      switch (ctx->phase) {
        case SWORDBOSS_PHASE0:
          m->a1 = 5.0;
          m->movement = movement_chase;
          actor_repeat(a, ACT2, 0.0, 0, 1.0);
          break;
        case SWORDBOSS_PHASE1:
          m->movement = movement_none;
          actor_repeat(a, ACT2, 1.0, 0, 0.2);
          actor_repeat(a, ACT3, 1.0, 0, 1.0);
          break;
      }
      break;
    case ACT2:
      // shooting action 1
      switch (ctx->phase) {
        case SWORDBOSS_PHASE0:
          shoot_linear(gs, &mr_swordboss_shooter, t->position, slow_fwd);
          break;
        case SWORDBOSS_PHASE1:
          ctx->rot_offset += 0.01;
          vector v = fdotv(6.0, normalize(mdotv(rotate_z(ctx->rot_offset), slow_fwd)));
          shoot_radial(gs, &mr_swordboss_shooter, t->position, v, 4);
          break;
      }
      break;
    case ACT3:
      shoot_shotgun(gs, &mr_swordboss_shooter2, t->position, slow_fwd, 5, M_PI / 3);
      break;
    }
    break;
  case EV_HIT:
    bullet_t *b = entity_get_component(gs, ev.entcol.e, bullet);
    h->hp -= b->damage;
    break;
  case EV_NO_HEALTH:
    entity_kill(gs, e);
    break;
  }
}