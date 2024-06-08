#include <game/boss.h>
#include <game/enemy.h>
#include <game/shoot.h>
#include <game/movement.h>
#include <game/game.h>
#include <stdio.h>
#include <stdbool.h>

struct bossgroupctx {
  boss_phase_t phase;
  entity_t swordboss;
  entity_t mageboss;
  entity_t scytheboss;
};

void mr_boss_group_invoke(game_t *gs
, entity_t e, event_t ev);
void mr_boss_group_phase_change(game_t *gs, entity_t e, boss_phase_t phase);

entity_t enemy_spawn_mr_boss_group(game_t *gs, vector pos)
{
  entity_t e = entity_add(gs, ENT_ANY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->position = pos;
  entity_add_component(gs, e, actor);
  entity_bind(gs, e, mr_boss_group_invoke);

  struct bossgroupctx *ctx = entity_get_context(gs, e, sizeof(struct bossgroupctx));
  
  ctx->phase = BOSS_PHASE_INACTIVE;

  ctx->scytheboss = enemy_spawn_mr_scytheboss(gs, vsubv(pos, vec2(2.0, 0.0)));
  ctx->mageboss = enemy_spawn_mr_mageboss(gs, vaddv(pos, vec2(0.0, 3.0)));
  ctx->swordboss = enemy_spawn_mr_swordboss(gs, vaddv(pos, vec2(2.0, 0.0)));

  gs->boss[0] = ctx->scytheboss;
  gs->boss[1] = ctx->mageboss;
  gs->boss[2] = ctx->swordboss;

  return e;
}

void mr_boss_group_invoke(game_t *gs, entity_t e, event_t ev)
{
  actor_t *a = entity_get_component(gs, e, actor);
  
  struct bossgroupctx *ctx = entity_get_context(gs, e, sizeof(struct bossgroupctx));

  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
      case ACT0:
        switch(ctx->phase) {
        case BOSS_PHASE_INACTIVE:
          mr_boss_group_phase_change(gs, e, BOSS_PHASE4);
          actor_do(a, ACT1, 0.0);
          break;
        case BOSS_PHASE0:
          mr_boss_group_phase_change(gs, e, BOSS_PHASE1);
          actor_do(a, ACT1, 0.0);
          break;
        case BOSS_PHASE1:
          mr_boss_group_phase_change(gs, e, BOSS_PHASE2);
          actor_do(a, ACT1, 0.0);
          break;
        case BOSS_PHASE2:
          mr_boss_group_phase_change(gs, e, BOSS_PHASE3);
          actor_do(a, ACT1, 0.0);
          break;
        case BOSS_PHASE3:
          mr_boss_group_phase_change(gs, e, BOSS_PHASE4);
          actor_do(a, ACT1, 0.0);
          break;
        case BOSS_PHASE4:
          mr_boss_group_phase_change(gs, e, BOSS_PHASE0);
          actor_do(a, ACT1, 0.0);
          break;
        }
        break;
      case ACT1:
        switch(ctx->phase) {
        case BOSS_PHASE_INACTIVE:
          break;
        case BOSS_PHASE0:
          actor_do(a, ACT0, 20.0);
          break;
        case BOSS_PHASE1:
          actor_do(a, ACT0, 20.0);
          break;
        case BOSS_PHASE2:
          actor_do(a, ACT0, 20.0);
          break;
        case BOSS_PHASE3:
          actor_do(a, ACT0, 30.0);
          break;
        case BOSS_PHASE4:
          actor_do(a, ACT0, 30.0);
          break;
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
    break;
  case EV_HIT:
  case EV_NO_HEALTH:
  case EV_MAP_COLLIDE:
  case EV_ENTITY_COLLIDE:
    break;
  }
}

void mr_boss_group_phase_change(game_t *gs, entity_t e, boss_phase_t phase)
{
  struct bossgroupctx *ctx = entity_get_context(gs, e, sizeof(struct bossgroupctx));
  
  actor_t *swordboss_a = entity_get_component(gs, ctx->swordboss, actor);
  actor_t *scytheboss_a = entity_get_component(gs, ctx->scytheboss, actor);
  actor_t *mageboss_a = entity_get_component(gs, ctx->mageboss, actor);

  bossctx_t *swordbossctx = entity_get_context(gs, ctx->swordboss, sizeof(bossctx_t));
  bossctx_t *scythebossctx = entity_get_context(gs, ctx->scytheboss, sizeof(bossctx_t));
  bossctx_t *magebossctx = entity_get_context(gs, ctx->mageboss, sizeof(bossctx_t));
  
  ctx->phase = phase;
  swordbossctx->phase = phase;
  scythebossctx->phase = phase;
  magebossctx->phase = phase;

  actor_stop_all(swordboss_a);
  actor_stop_all(scytheboss_a);
  actor_stop_all(mageboss_a);
  actor_do(swordboss_a, ACT0, 2.0);
  actor_do(scytheboss_a, ACT0, 2.0);
  actor_do(mageboss_a, ACT0, 2.0);
}