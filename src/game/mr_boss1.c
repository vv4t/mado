#include <game/enemy.h>
#include <game/shoot.h>
#include <game/movement.h>
#include <game/game.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum {
  SWORDBOSS_PHASE0,
  SWORDBOSS_PHASE1,
  SWORDBOSS_PHASE_ULT,
} swordboss_phase_t;

static const animation_t mr_swordboss_idle = { .tx = 4, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };
static const animation_t mr_swordboss_attack = { .tx = 4, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.20 };
static const animation_t mr_swordboss_charging = { .tx = 4, .ty = 2, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.5 };
static const animation_t mr_swordboss_ult = { .tx = 4, .ty = 0, .tw = 2, .th = 2, .framecount = 2, .frametime = 1.5 };

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
  bool              used_ult;
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
    actor_do(a, ACT0, 2.0);
    // play animation
  entity_add_component(gs, e, health);
    health_t *h = entity_get_component(gs, e, health);
    h->hp = 2000;
    h->max_hp = 2000;
  entity_add_component(gs, e, botmove);
    botmove_t *m = entity_get_component(gs, e, botmove);
    m->movement = movement_none;
  entity_bind(gs, e, mr_swordboss_invoke);
  return e;
}

vector flight_swordboss_curve(float time, float a1, float a2)
{
  return vec2(a1 * time, 1.0);
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
      actor_stop(a, ACT5);
      m->movement = movement_none;
      
      if (h->hp <= h->max_hp / 4) {
        ctx->phase = SWORDBOSS_PHASE_ULT;
        actor_do(a, ACT1, 0.0);
        break;
      }

      // switch phase
      switch (ctx->phase) {
        case SWORDBOSS_PHASE0:
          ctx->phase = SWORDBOSS_PHASE1;
          actor_do(a, ACT1, 0.0);
          break;
        case SWORDBOSS_PHASE1:
          ctx->phase = SWORDBOSS_PHASE0;
          actor_do(a, ACT1, 0.0);
          break;
        case SWORDBOSS_PHASE_ULT:
          // the ult phase lasts till death, so nothing needs to happen here.

          // ctx->phase = SWORDBOSS_PHASE0;
          // actor_do(a, ACT1, 3.0);
          break;
      }

      break;
    case ACT1:
      // phase start
      switch (ctx->phase) {
        case SWORDBOSS_PHASE0:
          actor_do(a, ACT0, 10.0);
          
          m->a1 = 5.0;
          m->movement = movement_chase;
          
          actor_repeat(a, ACT2, 0.0, 0, 1.0);
          break;
        case SWORDBOSS_PHASE1:
          actor_do(a, ACT0, 10.0);
          
          ctx->rot_offset = 0.0;
          
          m->a1 = 10.0;
          m->v1 = vec2(16.0, 16.0);
          m->movement = movement_travel;
          
          actor_repeat(a, ACT2, 1.0, 0, 0.1);
          actor_repeat(a, ACT3, 1.0, 0, 4.0);
          actor_repeat(a, ACT4, 1.0, 0, 1.0);
          break;
        case SWORDBOSS_PHASE_ULT:
          ctx->rot_offset = 0.0;
          
          m->a1 = 10.0;
          m->v1 = vec2(16.0, 16.0);
          m->movement = movement_travel;

          actor_repeat(a, ACT4, 0.0, 0, 0.1);
          break;
      }
      break;
    case ACT2:
      // shooting action 1
      switch (ctx->phase) {
        case SWORDBOSS_PHASE0:
          sprite_play(s, &mr_swordboss_attack);
          shoot_shotgun(gs, &mr_swordboss_shooter2, t->position, slow_fwd, 1.0, flight_accelerate, 4.0, 0.0, 20, M_PI + M_PI/3);
          break;
        case SWORDBOSS_PHASE1:
          if (t->position.x == 16.0 && t->position.y == 16.0) {
            ctx->rot_offset = gs->time / 3.0;
            vector v = fdotv(6.0, normalize(mdotv(rotate_z(ctx->rot_offset), vec2(0.0, 1.0))));
            shoot_radial(gs, &mr_swordboss_shooter, t->position, v, 1.0, flight_linear, 0.0, 0.0, 5);
          }
          break;
        case SWORDBOSS_PHASE_ULT:
          if (t->position.x == 16.0 && t->position.y == 16.0) {
            ctx->rot_offset = gs->time / 1.0;
            vector v = fdotv(6.0, normalize(mdotv(rotate_z(M_PI), slow_fwd)));
            shoot_shotgun(gs, &mr_swordboss_shooter, t->position, v, 1.0, flight_wave, 2.0, 0.0, 15, 6 * M_PI / 4);
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
            shoot_radial(gs, &mr_swordboss_shooter2, t->position, vec2(0.0, 3.0), 1.0, flight_swordboss_curve, 3.0, 0.0, 10);
            actor_do(a, ACT5, 0.3);
          }
          break;
        case SWORDBOSS_PHASE_ULT:
          if (t->position.x == 16.0 && t->position.y == 16.0) {
            int wall =  0b10001100000000;
            switch(rand() % 3) {
              case 0:
                wall |= 0b11101100000000;
                break;
              case 1:
                wall |= 0b10111100000000;
                break;
              case 2:
                wall |= 0b11011100000000;
                break;
            }
            shoot_wall(gs, &mr_swordboss_shooter2, t->position, slow_fwd, 1.0, flight_linear, 0.0, 0.0, wall, 14, 1.0);
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
            shoot_radial(gs, &mr_swordboss_shooter, t->position, vec2(0.0, 3.0), -1.0, flight_linear, 0.0, 0.0, 15);
          }
          break;
        case SWORDBOSS_PHASE_ULT:
          if (t->position.x == 16.0 && t->position.y == 16.0) {
            actor_stop(a, ACT4);
            sprite_repeat(s, &mr_swordboss_charging);
            actor_repeat(a, ACT2, 3.0, 0, 0.1);
            actor_repeat(a, ACT3, 6.0, 0, 0.5);
            actor_do(a, ACT5, 20.0);
            actor_do(a, ACT6, 5.0);
          }

          break;
      }
      break;
    case ACT5:
      switch (ctx->phase) {
        case SWORDBOSS_PHASE0:
          break;
        case SWORDBOSS_PHASE1:
          if (t->position.x == 16.0 && t->position.y == 16.0) {
            shoot_radial(gs, &mr_swordboss_shooter2, t->position, vec2(0.0, 3.0), -1.0, flight_swordboss_curve, 3.0, 0.0, 10);
          }
          break;
        case SWORDBOSS_PHASE_ULT:
          ctx->used_ult = true;
          break;
      }
      break;
    case ACT6:
      switch (ctx->phase) {
        case SWORDBOSS_PHASE0:
          break;
        case SWORDBOSS_PHASE1:
          if (t->position.x == 16.0 && t->position.y == 16.0) {
            shoot_radial(gs, &mr_swordboss_shooter2, t->position, vec2(0.0, 3.0), -1.0, flight_swordboss_curve, 3.0, 0.0, 10);
          }
          break;
        case SWORDBOSS_PHASE_ULT:
          sprite_repeat(s, &mr_swordboss_ult);
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
    // later, replace with death animation and entity_kill on a delay.

    if (ctx->used_ult) {
      // so we can't skip the ult phase by doing lethal dmg in normal phase
      entity_kill(gs, e);
    }
    break;
  }
}
