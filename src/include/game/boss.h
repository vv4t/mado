#ifndef BOSS_H
#define BOSS_H

typedef enum {
  BOSS_PHASE_INACTIVE,
  BOSS_PHASE0,
  BOSS_PHASE1,
  BOSS_PHASE2,
  BOSS_PHASE3,
  BOSS_PHASE4,
} boss_phase_t;

typedef struct bossctx {
  boss_phase_t phase;
} bossctx_t;

#endif
