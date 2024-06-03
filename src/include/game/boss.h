#ifndef BOSS_H
#define BOSS_H

typedef enum {
  BOSS_PHASE_INACTIVE,
  BOSS_PHASE0,
  BOSS_PHASE1,
} boss_phase_t;

typedef struct bossctx {
  boss_phase_t phase;
} bossctx_t;

#endif
