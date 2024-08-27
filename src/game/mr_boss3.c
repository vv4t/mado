#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <game/boss.h>
#include <stdio.h>
#include <stdbool.h>

static const animation_t mr_mageboss_idle = { .tx = 8, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };
static const animation_t mr_mageboss_attack = { .tx = 8, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.20 };

static shooter_t mr_mageboss_shooter = {
  .tx = 2, .ty = 0,
  .tw = 1, .th = 1,
  .target = ENT_PLAYER,
  .damage = 20
};

static shooter_t mr_mageboss_shooter2 = {
  .tx = 2, .ty = 3,
  .tw = 1, .th = 1,
  .target = ENT_PLAYER,
  .damage = 20
};

struct magebossctx {
  boss_phase_t phase;
  vector       dir;
};

void mr_mageboss_phase0_invoke(game_t *gs, entity_t e, event_t ev);
void mr_mageboss_phase1_invoke(game_t *gs, entity_t e, event_t ev);
void mr_mageboss_phase2_invoke(game_t *gs, entity_t e, event_t ev);
void mr_mageboss_phase3_invoke(game_t *gs, entity_t e, event_t ev);
void mr_mageboss_phase4_invoke(game_t *gs, entity_t e, event_t ev);
void mr_mageboss_phase_inactive_invoke(game_t *gs, entity_t e, event_t ev);
void mr_mageboss_invoke(game_t *gs, entity_t e, event_t ev);
vector flight_mr_mageboss_circle_origin(float time, float radius, float speed);
vector flight_mr_mageboss_circle(float time, float radius, float speed);

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
    h->hp = 20000;
    h->max_hp = 20000;
    h->invincible = true;
  entity_add_component(gs, e, npcmove);
    npcmove_t *bm = entity_get_component(gs, e, npcmove);
    npcmove_stop(bm);
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
    case BOSS_PHASE2:
      mr_mageboss_phase2_invoke(gs, e, ev);
      break;
    case BOSS_PHASE3:
      mr_mageboss_phase3_invoke(gs, e, ev);
      break;
    case BOSS_PHASE4:
      mr_mageboss_phase4_invoke(gs, e, ev);
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

void mr_mageboss_phase0_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);
  actor_t *a = entity_get_component(gs, e, actor);
  sprite_t *s = entity_get_component(gs, e, sprite);
  health_t *h = entity_get_component(gs, e, health);
  npcmove_t *bm = entity_get_component(gs, e, npcmove);

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;
      npcmove_travel(bm, vec2(24.0, 24.0), 10.0);
      actor_stop_all(a);
      actor_repeat(a, ACT1, 0.0, 0, 0.1);
      break;
    case ACT1:
      if (t->position.x == 24.0 && t->position.y == 24.0) {
        actor_stop(a, ACT1);
        actor_repeat(a, ACT2, 0.0, 0, 2.0);
        actor_repeat(a, ACT3, 1.0, 0, 2.0);
        actor_repeat(a, ACT4, 0.0, 45, 0.1);
      }
      break;
    case ACT2:
      sprite_play(s, &mr_mageboss_attack);
      shoot_radial(
        gs, 
        &mr_mageboss_shooter, 10.0,
        t->position, 
        vec2(0.0, 3.0), 1.0, 
        flight_linear, 0.0, 0.0, 
        15
      );
      break;
    case ACT3:
      sprite_play(s, &mr_mageboss_attack);
      vector v = mdotv(rotate_z(M_PI / 15), vec2(0.0, 3.0));
      shoot_radial(
        gs, 
        &mr_mageboss_shooter, 10.0,
        t->position, 
        v, 1.0, 
        flight_linear, 0.0, 0.0, 
        15
      );
      break;
    case ACT4:
      /*
      for (int i = 16.0; i < 22.0; i += 1.0) {
        shoot(
          gs, 
          &mr_mageboss_shooter, 15.5, 
          vec2(23.5, 24.5 - i),
          vec2(0.0, 2 * M_PI * i / 4.0), 1.0,
          flight_mr_mageboss_circle, i, 2 * M_PI * i / 4.0
        );
      }
      */
      break;
    case ACT5:
    case ACT6:
    case ACT7:
      break;
  }
}

void mr_mageboss_phase1_invoke(game_t *gs, entity_t e, event_t ev) {
  actor_t *a = entity_get_component(gs, e, actor);
  health_t *h = entity_get_component(gs, e, health);
  npcmove_t *bm = entity_get_component(gs, e, npcmove);
  sprite_t *s = entity_get_component(gs, e, sprite);
  transform_t *t = entity_get_component(gs, e, transform);

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;
      npcmove_travel(bm, vec2(26.0, 10.0), 20.0);
      actor_stop_all(a);
      actor_repeat(a, ACT1, 1.0, 0, 0.1);
      break;
    case ACT1:
      if (t->position.x == 26.0 && t->position.y == 10.0) {
        sprite_play(s, &mr_mageboss_attack);
        shoot_shotgun(
          gs, 
          &mr_mageboss_shooter, 5.0,
          vec2(24.0, 10.0), 
          mdotv(rotate_z(0.5 * sin(gs->time * 2.0)), vec2(0.0, -7.0)), 1.0, 
          flight_linear, 0.0, 0.0, 
          20, M_PI + M_PI / 4
        );
        shoot_wall(
          gs, 
          &mr_mageboss_shooter, 10.0,
          vec2(40.5, 32.0), 
          vec2(0.0, 15.0), 1.0, 
          flight_linear, 0.0, 0.0, 
          0b11111111111111111, 17, 1.0
        );
        shoot_wall(
          gs, 
          &mr_mageboss_shooter, 10.0,
          vec2(40.5, 32.0), 
          vec2(0.0, -15.0), 1.0, 
          flight_linear, 0.0, 0.0, 
          0b11111111111111111, 17, 1.0
        );
        shoot_wall(
          gs, 
          &mr_mageboss_shooter, 10.0,
          vec2(8.5, 32.0), 
          vec2(0.0, 15.0), 1.0, 
          flight_linear, 0.0, 0.0, 
          0b11111111111111111, 17, 1.0
        );
        shoot_wall(
          gs, 
          &mr_mageboss_shooter, 10.0,
          vec2(8.5, 32.0), 
          vec2(0.0, -15.0), 1.0, 
          flight_linear, 0.0, 0.0, 
          0b11111111111111111, 17, 1.0
        );
        shoot_linear(gs, &mr_mageboss_shooter, 5.0, vec2(45.5, 28.0), vec2(0.0, -15.0));
        shoot_linear(gs, &mr_mageboss_shooter, 5.0, vec2(1.5, 28.0), vec2(0.0, -15.0));
      }
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

void mr_mageboss_phase2_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);
  actor_t *a = entity_get_component(gs, e, actor);
  health_t *h = entity_get_component(gs, e, health);
  npcmove_t *bm = entity_get_component(gs, e, npcmove);
  sprite_t *s = entity_get_component(gs, e, sprite);

  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  vector to_player = normalize(vsubv(pt->position, t->position));

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;
      npcmove_travel(bm, vec2(23.5, 24.0), 10.0);
      actor_stop_all(a);
      actor_repeat(a, ACT1, 0.0, 0, 0.1);
      break;
    case ACT1:
      if (t->position.x == 23.5 && t->position.y == 24.0) {
        sprite_play(s, &mr_mageboss_attack);
        vector v = mdotv(rotate_z(gs->time / 3.0), vec2(0.0, 5.0));
        shoot_radial(
          gs, 
          &mr_mageboss_shooter, 5.0,
          vec2(24.0, 24.0), 
          v, 1.0, 
          flight_linear, 0.0, 0.0, 
          5
        );
      }
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

void mr_mageboss_phase3_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);
  actor_t *a = entity_get_component(gs, e, actor);
  health_t *h = entity_get_component(gs, e, health);
  npcmove_t *bm = entity_get_component(gs, e, npcmove);
  sprite_t *s = entity_get_component(gs, e, sprite);

  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  vector to_player = normalize(vsubv(pt->position, t->position));

  struct magebossctx *ctx = entity_get_context(gs, e, sizeof(struct magebossctx));

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;
      npcmove_travel(bm, vec2(23.5, 24.0), 10.0);
      actor_stop_all(a);
      actor_repeat(a, ACT1, 0.0, 0, 0.1);
      break;
    case ACT1:
      if (t->position.x == 23.5 && t->position.y == 24.0) {
        actor_stop(a, ACT1);
        ctx->dir = to_player;
        actor_repeat(a, ACT2, 0.0, 20, 0.2);
      }
      break;
    case ACT2:
      for (float i = 1.0; i < 27.0; i += 1.0) {
        shoot(
          gs, 
          &mr_mageboss_shooter, 25.0, 
          vec2(24.0, 24.0),
          fdotv(2 * M_PI * i / 5.0, ctx->dir), 1.0,
          flight_mr_mageboss_circle_origin, i, 2 * M_PI * i / 5.0
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

void mr_mageboss_phase4_invoke(game_t *gs, entity_t e, event_t ev) {
  transform_t *t = entity_get_component(gs, e, transform);
  actor_t *a = entity_get_component(gs, e, actor);
  health_t *h = entity_get_component(gs, e, health);
  npcmove_t *bm = entity_get_component(gs, e, npcmove);
  sprite_t *s = entity_get_component(gs, e, sprite);

  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  vector to_player = normalize(vsubv(pt->position, t->position));

  switch(ev.act.name) {
    case ACT0:
      h->invincible = false;
      npcmove_travel(bm, vec2(24.0, 16.0), 10.0);
      actor_stop_all(a);
      actor_repeat(a, ACT1, 0.0, 0, 0.1);
      break;
    case ACT1:
      if (t->position.x == 24.0 && t->position.y == 16.0) {
        actor_stop(a, ACT1);
        actor_repeat(a, ACT2, 1.0, 0, 1.0);
        actor_repeat(a, ACT4, 4.0, 0, 4.0);
        actor_do(a, ACT5, 20.0);
      }
      break;
    case ACT2:
      sprite_play(s, &mr_mageboss_attack);
      actor_do(a, ACT3, 0.3);
      break;
    case ACT3:
      vector v = mdotv(rotate_z(gs->time / 3.0), vec2(0.0, 2.0));
      shoot_radial(
        gs, 
        &mr_mageboss_shooter, 10.0,
        t->position, 
        v, 1.0, 
        flight_linear, 1.0, 22.0, 
        10
      );
      break;
    case ACT4:
      shoot_radial(
        gs, 
        &mr_mageboss_shooter2, 10.0,
        t->position, 
        vec2(0.0, 3.0), 1.0, 
        flight_wave, 1.0, 22.0, 
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

vector flight_mr_mageboss_circle_origin(float time, float radius, float speed) {
  float t1 = radius / speed;
  float t2 = time * 32.0;
  
  if (time < t1) {
    return vec4(0, -1, cos(t2), sin(t2));
  }
  
  time -= t1;
  
  float r = speed / radius;
  
  float vx = cos(time * r);
  float vy = sin(time * r);
  
  return vec4(vx, vy, vx, vy);
}

vector flight_mr_mageboss_circle(float time, float radius, float speed) {
  float r = speed / radius;
  
  float vx = cos(time * r);
  float vy = sin(time * r);
  
  return vec4(vx, vy, vx, vy);
}
