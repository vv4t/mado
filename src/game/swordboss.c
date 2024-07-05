/*
#include <game/enemy.h>
#include <game/shoot.h>
#include <game/movement.h>
#include <game/game.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum {
  SWORDBOSS_PHASE_INACTIVE,
  SWORDBOSS_PHASE0,
  SWORDBOSS_PHASE1,
  SWORDBOSS_PHASE_ULT,
  SWORDBOSS_PHASE_FELL
} swordboss_phase_t;

static const animation_t mr_swordboss_idle = { .tx = 4, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };
static const animation_t mr_swordboss_attack = { .tx = 4, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.20 };
static const animation_t mr_swordboss_charging = { .tx = 4, .ty = 2, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };
static const animation_t mr_swordboss_ult = { .tx = 4, .ty = 0, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };
static const animation_t mr_swordboss_fall = { .tx = 4, .ty = 8, .tw = 2, .th = 2, .framecount = 2, .frametime = 2.0 };

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
  bool              at_ult_hp;
};

void mr_swordboss_phase_change(game_t *gs, entity_t e);
void mr_swordboss_phase0_invoke(game_t *gs, entity_t e, event_t ev);
void mr_swordboss_phase1_invoke(game_t *gs, entity_t e, event_t ev);
void mr_swordboss_phase_ult_invoke(game_t *gs, entity_t e, event_t ev);
void mr_swordboss_phase_fell_invoke(game_t *gs, entity_t e, event_t ev);
void mr_swordboss_phase_inactive_invoke(game_t *gs, entity_t e, event_t ev);
vector flight_swordboss_curve(float time, float radius, float speed);
void mr_swordboss_invoke(game_t *gs, entity_t e, event_t ev);

entity_t enemy_spawn_mr_swordboss(game_t *gs, vector pos)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.5, 2.5, 2.5);
    // t->position = vec2(48/2 - 4, 4 * 48/6);
    t->position = pos;
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_swordboss_idle);
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
  entity_bind(gs, e, mr_swordboss_invoke);

  struct swordbossctx *ctx = entity_get_context(gs, e, sizeof(struct swordbossctx));
  ctx->phase = SWORDBOSS_PHASE_INACTIVE;

  return e;
}

vector flight_swordboss_curve(float time, float a1, float a2)
{
  return vec4(a1 * time, 1.0, a1 * time, 1.0);
}

void mr_swordboss_invoke(game_t *gs, entity_t e, event_t ev)
{
  sprite_t *s = entity_get_component(gs, e, sprite);
  actor_t *a = entity_get_component(gs, e, actor);
  health_t *h = entity_get_component(gs, e, health);
  
  struct swordbossctx *ctx = entity_get_context(gs, e, sizeof(struct swordbossctx));

  switch (ev.type) {
  case EV_ACT:
    switch(ctx->phase) {
    case SWORDBOSS_PHASE_INACTIVE:
      mr_swordboss_phase_inactive_invoke(gs, e, ev);
      break;
    case SWORDBOSS_PHASE0:
      mr_swordboss_phase0_invoke(gs, e, ev);
      break;
    case SWORDBOSS_PHASE1:
      mr_swordboss_phase1_invoke(gs, e, ev);
      break;
    case SWORDBOSS_PHASE_ULT:
      mr_swordboss_phase_ult_invoke(gs, e, ev);
      break;
    case SWORDBOSS_PHASE_FELL:
      mr_swordboss_phase_fell_invoke(gs, e, ev);
      break;
    }
    break;
  case EV_HIT:
    bullet_t *b = entity_get_component(gs, ev.col.e, bullet);
    health_damage(h, b->damage);
    if (h->hp <= h->max_hp / 4 && ctx->at_ult_hp == false) {
      ctx->at_ult_hp = true;
      h->invincible = true;
    }    
    break;
  case EV_NO_HEALTH:
    if (ctx->phase != SWORDBOSS_PHASE_FELL) {
      ctx->phase = SWORDBOSS_PHASE_FELL;
      sprite_repeat(s, &mr_swordboss_fall);
      actor_stop_all(a);
      actor_do(a, ACT0, 4.0);
    }
    break;
  case EV_MAP_COLLIDE:
    break;
  case EV_ENTITY_COLLIDE:
    break;
  }
}

void mr_swordboss_phase_change(game_t *gs, entity_t e) {
  actor_t *a = entity_get_component(gs, e, actor);
  botmove_t *bm = entity_get_component(gs, e, botmove);

  struct swordbossctx *ctx = entity_get_context(gs, e, sizeof(struct swordbossctx));

  actor_stop_all(a);
  botmove_stop(bm);

  if (ctx->at_ult_hp) {
    ctx->phase = SWORDBOSS_PHASE_ULT;
  } else {
    switch (ctx->phase) {
      case SWORDBOSS_PHASE0:
        ctx->phase = SWORDBOSS_PHASE1;
        break;
      case SWORDBOSS_PHASE1:
        ctx->phase = SWORDBOSS_PHASE0;
        break;
      case SWORDBOSS_PHASE_ULT:
        break;
      case SWORDBOSS_PHASE_FELL:
        break;
      case SWORDBOSS_PHASE_INACTIVE:
        break;
    }
  }
  
  actor_do(a, ACT1, 0.0);
}

void mr_swordboss_phase0_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);
  sprite_t *s = entity_get_component(gs, e, sprite);
  actor_t *a = entity_get_component(gs, e, actor);
  botmove_t *bm = entity_get_component(gs, e, botmove);
  health_t *h = entity_get_component(gs, e, health);
  
  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  vector to_player = normalize(vsubv(pt->position, t->position));

  switch(ev.act.name) {
    case ACT0:
      mr_swordboss_phase_change(gs, e);
      break;
    case ACT1:
      h->invincible = false;
      actor_do(a, ACT0, 10.0);
      botmove_chase(bm, 5.0);
      actor_repeat(a, ACT2, 0.0, 0, 1.0);
      break;
    case ACT2:
      sprite_play(s, &mr_swordboss_attack);
      shoot_shotgun(
        gs, 
        &mr_swordboss_shooter2, 
        t->position, 
        fdotv(3.0, to_player), 1.0, 
        flight_accelerate, 4.0, 0.0, 
        20, M_PI + M_PI/3
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

void mr_swordboss_phase1_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);
  sprite_t *s = entity_get_component(gs, e, sprite);
  actor_t *a = entity_get_component(gs, e, actor);
  botmove_t *bm = entity_get_component(gs, e, botmove);
  health_t *h = entity_get_component(gs, e, health);

  switch(ev.act.name) {
    case ACT0:
      mr_swordboss_phase_change(gs, e);
      break;
    case ACT1:
      h->invincible = false;
      actor_do(a, ACT0, 10.0);
      botmove_travel(bm, vec2(24.0, 24.0), 10.0);
      actor_repeat(a, ACT2, 1.0, 0, 0.1);
      actor_repeat(a, ACT3, 1.0, 0, 1.0);
      actor_repeat(a, ACT4, 1.0, 0, 2.0);
      break;
    case ACT2:
      if (t->position.x == 24.0 && t->position.y == 24.0) {
        vector v = mdotv(rotate_z(gs->time / 3.0), vec2(0.0, 6.0));
        shoot_radial(
          gs, 
          &mr_swordboss_shooter, 
          t->position, 
          v, 1.0, 
          flight_linear, 0.0, 0.0, 
          5
        );
      }
      break;
    case ACT3:
      if (t->position.x == 24.0 && t->position.y == 24.0) {
        shoot_radial(
          gs, 
          &mr_swordboss_shooter, 
          t->position, 
          vec2(0.0, 3.0), -1.0, 
          flight_linear, 0.0, 0.0, 
          15
        );
      }
      break;
    case ACT4:
      if (t->position.x == 24.0 && t->position.y == 24.0) {
        sprite_play(s, &mr_swordboss_attack);
        shoot_radial(
          gs, 
          &mr_swordboss_shooter2, 
          t->position, 
          vec2(0.0, 3.0), 1.0, 
          flight_swordboss_curve, 3.0, 0.0, 
          10
        );
        actor_do(a, ACT5, 0.3);
      }
      break;
    case ACT5:
      if (t->position.x == 24.0 && t->position.y == 24.0) {
        shoot_radial(
          gs, 
          &mr_swordboss_shooter2, 
          t->position, 
          vec2(0.0, 3.0), -1.0, 
          flight_swordboss_curve, 3.0, 0.0, 
          10
        );
      }
      break;
    case ACT6:
    case ACT7:
      break;
  }
}

void mr_swordboss_phase_ult_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);
  sprite_t *s = entity_get_component(gs, e, sprite);
  actor_t *a = entity_get_component(gs, e, actor);
  botmove_t *bm = entity_get_component(gs, e, botmove);
  health_t *h = entity_get_component(gs, e, health);

  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  vector to_player = normalize(vsubv(pt->position, t->position));

  switch(ev.act.name) {
    case ACT0:
      break;
    case ACT1:
      botmove_travel(bm, vec2(24.0, 24.0), 10.0);
      actor_repeat(a, ACT4, 0.0, 0, 0.1);
      break;
    case ACT2:
      if (t->position.x == 24.0 && t->position.y == 24.0) {
        vector v = fdotv(6.0, normalize(mdotv(rotate_z(M_PI), to_player)));
        shoot_shotgun(
          gs, 
          &mr_swordboss_shooter, 
          t->position, 
          v, 1.0, 
          flight_wave, 2.0, 0.0, 
          15, 6 * M_PI / 4
        );
      }
      break;
    case ACT3:
      if (t->position.x == 24.0 && t->position.y == 24.0) {
        int wall =  0b10001111000000;
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
        shoot_wall(
          gs, 
          &mr_swordboss_shooter2, 
          t->position, 
          fdotv(3.0, to_player), 1.0, 
          flight_linear, 0.0, 0.0, 
          wall, 14, 1.0
        );
      }
      break;
    case ACT4:
      if (t->position.x == 24.0 && t->position.y == 24.0) {
        actor_stop(a, ACT4);

        sprite_repeat(s, &mr_swordboss_charging);
        actor_repeat(a, ACT2, 3.0, 0, 0.1);
        actor_repeat(a, ACT3, 6.0, 0, 0.5);
        actor_do(a, ACT5, 20.0);
        actor_do(a, ACT6, 5.0);
      }
      break;
    case ACT5:
      h->invincible = false;
      break;
    case ACT6:
      sprite_repeat(s, &mr_swordboss_ult);
      break;
    case ACT7:
      break;
  }
}

void mr_swordboss_phase_fell_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);

  switch(ev.act.name) {
    case ACT0:
      shoot_radial(
        gs, 
        &mr_swordboss_shooter, 
        t->position, 
        vec2(3.0, 0.0), 1.0, 
        flight_linear, 0.0, 0.0, 
        20
      );
      entity_kill(gs, e);
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

void mr_swordboss_phase_inactive_invoke(game_t *gs, entity_t e, event_t ev) {
  health_t *h = entity_get_component(gs, e, health);
  actor_t *a = entity_get_component(gs, e, actor);
  sprite_t *s = entity_get_component(gs, e, sprite);
  botmove_t *bm = entity_get_component(gs, e, botmove);
  transform_t *t = entity_get_component(gs, e, transform);

  struct swordbossctx *ctx = entity_get_context(gs, e, sizeof(struct swordbossctx));

  switch(ev.act.name) {
    case ACT0:
      actor_repeat(a, ACT2, 0.0, 0, 0.1);
      botmove_travel(bm, vec2(24.0, 24.0), 5.0);
      break;
    case ACT1:
      break;
    case ACT2:
      if (t->position.x == 24.0 && t->position.y == 24.0) {
        actor_stop(a, ACT2);

        h->invincible = false;
  
        ctx->phase = SWORDBOSS_PHASE1;
        actor_do(a, ACT1, 2.0);
        sprite_repeat(s, &mr_swordboss_idle);
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
*/