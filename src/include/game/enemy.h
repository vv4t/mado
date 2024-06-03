#ifndef ENEMY_H
#define ENEMY_H

#include <lib/math3d.h>

struct game_s;

int enemy_spawn_mr_warrior(struct game_s *gs);
int enemy_spawn_mr_mage(struct game_s *gs);
int enemy_spawn_mr_scythe(struct game_s *gs);
int enemy_spawn_mr_dummy(struct game_s *gs);
int enemy_spawn_mr_swordboss(struct game_s *gs, vector pos);
int enemy_spawn_mr_scytheboss(struct game_s *gs, vector pos);
int enemy_spawn_mr_mageboss(struct game_s *gs, vector pos);
int enemy_spawn_mr_boss_group(struct game_s *gs, vector pos);


#endif
