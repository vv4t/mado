#ifndef ENEMY_H
#define ENEMY_H

struct game_s;

int enemy_spawn_mr_warrior(struct game_s *gs);
int enemy_spawn_mr_mage(struct game_s *gs);
int enemy_spawn_mr_scythe(struct game_s *gs);
int enemy_spawn_mr_dummy(struct game_s *gs);
int enemy_spawn_mr_swordboss(struct game_s *gs);

#endif
