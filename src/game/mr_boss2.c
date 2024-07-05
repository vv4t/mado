#include <game/enemy.h>
#include <game/shoot.h>
#include <game/movement.h>
#include <game/game.h>
#include <game/boss.h>
#include <stdio.h>
#include <stdbool.h>

static const animation_t mr_scytheboss_idle = { .tx = 12, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };
static const animation_t mr_scytheboss_attack = { .tx = 12, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.30 };

static shooter_t mr_scytheboss_shooter = {
  .tx = 1, .ty = 0,
  .tw = 1, .th = 1,
  .target = ENT_PLAYER,
  .damage = 20
};

static shooter_t mr_scytheboss_shooter2 = {
  .tx = 1, .ty = 3,
  .tw = 1, .th = 1,
  .target = ENT_PLAYER,
  .damage = 20
};

void mr_scytheboss_phase0_invoke(game_t *gs, entity_t e, event_t ev);
void mr_scytheboss_phase1_invoke(game_t *gs, entity_t e, event_t ev);
void mr_scytheboss_phase2_invoke(game_t *gs, entity_t e, event_t ev);
void mr_scytheboss_phase3_invoke(game_t *gs, entity_t e, event_t ev);
void mr_scytheboss_phase4_invoke(game_t *gs, entity_t e, event_t ev);
void mr_scytheboss_phase_inactive_invoke(game_t *gs, entity_t e, event_t ev);
void mr_scytheboss_invoke(game_t *gs, entity_t e, event_t ev);
vector flight_scytheboss_wave(float time, float a1, float a2);
vector flight_scytheboss_linear(float time, float a1, float a2);

entity_t enemy_spawn_mr_scytheboss(game_t *gs, vector pos)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.5, 2.5, 2.5);
    t->position = pos;
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_scytheboss_idle);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.8;
  entity_add_component(gs, e, actor);
  entity_add_component(gs, e, health);
    health_t *h = entity_get_component(gs, e, health);
    h->hp = 20000;
    h->max_hp = 20000;
    h->invincible = true;
  entity_add_component(gs, e, botmove);
    botmove_t *bm = entity_get_component(gs, e, botmove);
    botmove_stop(bm);
  entity_bind(gs, e, mr_scytheboss_invoke);

  bossctx_t *ctx = entity_get_context(gs, e, sizeof(bossctx_t));
  ctx->phase = BOSS_PHASE_INACTIVE;

  return e;
}

void mr_scytheboss_invoke(game_t *gs, entity_t e, event_t ev)
{
  health_t *h = entity_get_component(gs, e, health);
  
  bossctx_t *ctx = entity_get_context(gs, e, sizeof(bossctx_t));

  switch (ev.type) {
  case EV_ACT:
    switch(ctx->phase) {
    case BOSS_PHASE_INACTIVE:
      mr_scytheboss_phase_inactive_invoke(gs, e, ev);
      break;
    case BOSS_PHASE0:
      mr_scytheboss_phase0_invoke(gs, e, ev);
      break;
    case BOSS_PHASE1:
      mr_scytheboss_phase1_invoke(gs, e, ev);
      break;
    case BOSS_PHASE2:
      mr_scytheboss_phase2_invoke(gs, e, ev);
      break;
    case BOSS_PHASE3:
      mr_scytheboss_phase3_invoke(gs, e, ev);
      break;
    case BOSS_PHASE4:
      mr_scytheboss_phase4_invoke(gs, e, ev);
      break;
    }
    break;
  case EV_HIT:
    bullet_t *b = entity_get_component(gs, ev.col.e, bullet);
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

void mr_scytheboss_phase0_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);
  actor_t *a = entity_get_component(gs, e, actor);
  sprite_t *s = entity_get_component(gs, e, sprite);
  health_t *h = entity_get_component(gs, e, health);
  botmove_t *bm = entity_get_component(gs, e, botmove);

  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  vector to_player = normalize(vsubv(pt->position, t->position));

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;

      vector pivot = vec2(24.0, 24.0);
      vector closest = vaddv(fdotv(12.0, normalize(vsubv(t->position, pivot))), pivot);
      botmove_travel(bm, closest, 5.0);

      actor_stop_all(a);
      actor_repeat(a, ACT3, 0.0, 0, 0.1);

      break;
    case ACT1:
      sprite_play(s, &mr_scytheboss_attack);
      actor_do(a, ACT2, 0.3);
      break;
    case ACT2:
      shoot_radial(
        gs, 
        &mr_scytheboss_shooter, 5.0,
        t->position, 
        fdotv(5.0, to_player), 1.0, 
        flight_scytheboss_wave, 1.0, 22.0, 
        15
      );
      break;
    case ACT3:
      float dist = length(vsubv(vec2(24.0, 24.0), t->position));
      if (dist <= 12.0 + 5.0 * 0.015 && dist >= 12.0 - 5.0 * 0.015) {
        actor_stop(a, ACT3);
        actor_repeat(a, ACT1, 1.0, 0, 2.0);
        botmove_orbit(bm, vsubv(vec2(24.0, 24.0), t->position), 15.0);
      }
      break;
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
  botmove_t *bm = entity_get_component(gs, e, botmove);
  sprite_t *s = entity_get_component(gs, e, sprite);

  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  vector to_player = normalize(vsubv(pt->position, t->position));

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;
      botmove_travel(bm, vec2(22.0, 10.0), 20.0);
      actor_stop_all(a);
      actor_repeat(a, ACT3, 0.0, 0, 0.1);
      break;
    case ACT1:
      sprite_play(s, &mr_scytheboss_attack);
      shoot_wall(
        gs, 
        &mr_scytheboss_shooter, 10.0,
        t->position, 
        vec2(0.0, 5.0), 1.0, 
        flight_scytheboss_linear, 2.0, 1.0, 
        0b0001010101010101010100000, 25, 1.0
      );
      break;
    case ACT2:
      sprite_play(s, &mr_scytheboss_attack);
      shoot_wall(
        gs, 
        &mr_scytheboss_shooter, 10.0,
        t->position, 
        vec2(0.0, 5.0), 1.0, 
        flight_scytheboss_linear, 2.0, 1.0, 
        0b0000101010101010101000000, 25, 1.0
      );
    case ACT3:
      if (t->position.x == 22.0 && t->position.y == 10.0) {
        actor_stop(a, ACT3);
        actor_repeat(a, ACT1, 0.75, 0, 1.5);
        actor_repeat(a, ACT2, 1.5, 0, 1.5);
      }
      break;
    case ACT4:
    case ACT5:
    case ACT6:
    case ACT7:
      break;
  }
}

void mr_scytheboss_phase2_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);
  actor_t *a = entity_get_component(gs, e, actor);
  health_t *h = entity_get_component(gs, e, health);
  botmove_t *bm = entity_get_component(gs, e, botmove);
  sprite_t *s = entity_get_component(gs, e, sprite);

  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  vector to_player = normalize(vsubv(pt->position, t->position));

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;

      vector pivot = vec2(24.0, 24.0);
      vector closest = vaddv(fdotv(2.0, normalize(vsubv(t->position, pivot))), pivot);
      botmove_travel(bm, closest, 5.0);

      actor_stop_all(a);
      actor_repeat(a, ACT7, 0.0, 0, 0.1);
      break;
    case ACT1:
      sprite_play(s, &mr_scytheboss_attack);
      shoot_radial(
        gs, 
        &mr_scytheboss_shooter, 10.0,
        t->position, 
        mdotv(rotate_z(gs->time * 5), vec2(0.0, 3.0)), 1.0, 
        flight_scytheboss_linear, 0.0, 0.0, 
        5
      );
      break;
    case ACT2:
    case ACT3:
    case ACT4:
    case ACT5:
    case ACT6:
      break;
    case ACT7:
      float dist = length(vsubv(vec2(24.0, 24.0), t->position));
      if (dist <= 2.0 + 5.0 * 0.015 && dist >= 2.0 - 5.0 * 0.015) {
        actor_stop(a, ACT7);
        actor_repeat(a, ACT1, 0.0, 0, 0.3);
        botmove_orbit(bm, vsubv(vec2(24.0, 24.0), t->position), 9.0);
      }
      break;
  }
}

void mr_scytheboss_phase3_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);
  actor_t *a = entity_get_component(gs, e, actor);
  health_t *h = entity_get_component(gs, e, health);
  botmove_t *bm = entity_get_component(gs, e, botmove);
  sprite_t *s = entity_get_component(gs, e, sprite);

  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  vector to_player = normalize(vsubv(pt->position, t->position));

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;

      vector pivot = vec2(24.0, 24.0);
      vector closest = vaddv(fdotv(2.0, normalize(vsubv(t->position, pivot))), pivot);
      botmove_travel(bm, closest, 5.0);

      actor_stop_all(a);
      actor_repeat(a, ACT3, 0.0, 0, 0.1);
      break;
    case ACT1:
      sprite_play(s, &mr_scytheboss_attack);
      actor_do(a, ACT2, 0.3);
      break;
    case ACT2:
      shoot_radial(
        gs, 
        &mr_scytheboss_shooter, 5.0,
        t->position, 
        fdotv(3.0, to_player), 1.0, 
        flight_scytheboss_linear, 1.0, 22.0, 
        10
      );
      break;
    case ACT3:
      float dist = length(vsubv(vec2(24.0, 24.0), t->position));
      if (dist <= 2.0 + 5.0 * 0.015 && dist >= 2.0 - 5.0 * 0.015) {
        actor_stop(a, ACT3);
        actor_repeat(a, ACT1, 1.0, 0, 2.0);
        actor_repeat(a, ACT4, 1.0, 0, 1.0);
        botmove_orbit(bm, vsubv(vec2(24.0, 24.0), t->position), 9.0);
      }
      break;
    case ACT4:
      shoot_radial(
        gs, 
        &mr_scytheboss_shooter, 5.0,
        t->position, 
        fdotv(4.0, to_player), 1.0, 
        flight_scytheboss_linear, 1.0, 22.0, 
        4
      );
      break;
    case ACT5:
    case ACT6:
    case ACT7:
      break;
  }
}

void mr_scytheboss_phase4_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);
  actor_t *a = entity_get_component(gs, e, actor);
  health_t *h = entity_get_component(gs, e, health);
  botmove_t *bm = entity_get_component(gs, e, botmove);
  sprite_t *s = entity_get_component(gs, e, sprite);

  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  vector to_player = normalize(vsubv(pt->position, t->position));

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;
      botmove_travel(bm, vec2(16.0, 32.0), 10.0);
      actor_stop_all(a);
      actor_repeat(a, ACT1, 0.0, 0, 0.1);
      break;
    case ACT1:
      if (t->position.x == 16.0 && t->position.y == 32.0) {
        actor_stop(a, ACT1);
        actor_repeat(a, ACT2, 1.0, 0, 1.0);
        actor_repeat(a, ACT4, 4.0, 0, 4.0);
        actor_do(a, ACT5, 20.0);
      }
      break;
    case ACT2:
      sprite_play(s, &mr_scytheboss_attack);
      actor_do(a, ACT3, 0.3);
      break;
    case ACT3:
      vector v = mdotv(rotate_z(gs->time / 3.0), vec2(0.0, 2.0));
      shoot_radial(
        gs, 
        &mr_scytheboss_shooter, 10.0,
        t->position, 
        v, 1.0, 
        flight_scytheboss_linear, 1.0, 22.0, 
        10
      );
      break;
    case ACT4:
      shoot_radial(
        gs, 
        &mr_scytheboss_shooter2, 10.0,
        t->position, 
        vec2(0.0, 3.0), 1.0, 
        flight_scytheboss_wave, 1.0, 22.0, 
        10
      );
      break;
    case ACT5:
      actor_stop_all(a);
      break;
    case ACT6:
    case ACT7:
      break;
  }
}

void mr_scytheboss_phase_inactive_invoke(game_t *gs, entity_t e, event_t ev) {
  health_t *h = entity_get_component(gs, e, health);
  actor_t *a = entity_get_component(gs, e, actor);
  sprite_t *s = entity_get_component(gs, e, sprite);

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;
      sprite_repeat(s, &mr_scytheboss_idle);
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

vector flight_scytheboss_wave(float time, float a1, float a2)
{
  return vec4(cos(time * a1 + a2), 1.0, -cos(20 * time), sin(20 * time));
}

vector flight_scytheboss_linear(float time, float a1, float a2)
{
  return vec4(0.0, 1.0, -cos(20 * time), sin(20 * time));
}