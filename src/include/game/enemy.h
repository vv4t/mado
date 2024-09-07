#ifndef ENEMY_H
#define ENEMY_H

#include <lib/math3d.h>

typedef struct game_s game_t;

void enemy_spawn_mr_warrior(game_t *gs, vector position);
void enemy_spawn_mr_mage(game_t *gs, vector position);
void enemy_spawn_mr_shadow(game_t *gs, vector position);
void enemy_spawn_mr_small_warrior(game_t *gs, vector spawn_pos);
void enemy_spawn_mr_small_mage(game_t *gs, vector spawn_pos);
void enemy_spawn_mr_fire(game_t *gs, vector spawn_pos);
void enemy_spawn_mr_phoenix(game_t *gs, vector spawn_pos);

#endif
