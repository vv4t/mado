#ifndef ENEMY_H
#define ENEMY_H

#include <lib/math3d.h>

typedef struct game_s game_t;

int enemy_spawn_mr_warrior(game_t *gs, vector spawn_pos);
int enemy_spawn_mr_fire(game_t *gs, vector spawn_pos);

#endif
