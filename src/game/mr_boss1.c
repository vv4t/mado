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
static const animation_t mr_swordboss_attack = { .tx = 4, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.20 };

static shooter_t mr_swordboss_shooter = {
  .tx = 2, .ty = 0,
  .tw = 1, .th = 1,
  .ttl = 5.0,
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

vector flight_swordboss_curve(float time, float radius, float speed);
void mr_swordboss_invoke(game_t *gs, entity_t e, event_t ev);

entity_t enemy_spawn_mr_swordboss(game_t *gs)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.0, 2.0, 2.0);
    t->position = vec2(16, 16);
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_swordboss_idle);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.8;
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_repeat(a, ACT0, 2.0, 0, 16.0);
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

vector flight_swordboss_curve(float time, float a1, float a2)
{
  return vec2(3 * time, 1.0);
}

void mr_swordboss_invoke(game_t *gs, entity_t e, event_t ev)
{
  const transform_t *pt = entity_get_component(gs, gs->player, transform);

  transform_t *t = entity_get_component(gs, e, transform);
  sprite_t *s = entity_get_component(gs, e, sprite);
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
      actor_stop(a, ACT4);
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
          ctx->rot_offset = 0.0;
          m->a1 = 10.0;
          m->v1 = vec2(16.0, 16.0);
          m->movement = movement_travel;
          actor_repeat(a, ACT2, 1.0, 0, 0.1);
          actor_repeat(a, ACT3, 1.0, 0, 4.0);
          break;
      }
      break;
    case ACT2:
      // shooting action 1
      switch (ctx->phase) {
        case SWORDBOSS_PHASE0:
          sprite_play(s, &mr_swordboss_attack);
          shoot_shotgun(gs, &mr_swordboss_shooter2, t->position, slow_fwd, 1.0, flight_accelerate, 4.0, 0.0, 5, M_PI / 2);
          break;
        case SWORDBOSS_PHASE1:
          if (t->position.x == 16.0 && t->position.y == 16.0) {
            ctx->rot_offset = gs->time / 3.0;
            vector v = fdotv(6.0, normalize(mdotv(rotate_z(ctx->rot_offset), vec2(0.0, 1.0))));
            shoot_radial(gs, &mr_swordboss_shooter, t->position, v, 1.0, flight_linear, 0.0, 0.0, 30);
          }
          break;
      }
      break;
    case ACT3:
      switch (ctx->phase) {
        case SWORDBOSS_PHASE0:
          break;
        case SWORDBOSS_PHASE1:
          if (t->position.x == 16.0 && t->position.y == 16.0) {
            sprite_play(s, &mr_swordboss_attack);
            shoot_radial(gs, &mr_swordboss_shooter2, t->position, vec2(0.0, 3.0), 1.0, flight_linear, 5.0, 0.0, 20);
            actor_do(a, ACT4, 0.3);
          }
          break;
      }
      break;
    case ACT4:
      switch (ctx->phase) {
        case SWORDBOSS_PHASE0:
          break;
        case SWORDBOSS_PHASE1:
          if (t->position.x == 16.0 && t->position.y == 16.0) {
            shoot_radial(gs, &mr_swordboss_shooter2, t->position, vec2(0.0, 3.0), -1.0, flight_linear, 5.0, 0.0, 20);
          }
          break;
      }
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
