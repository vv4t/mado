#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <game/game.h>
#include <lib/math3d.h>

vector movement_chase(game_t *gs, entity_t e, float speed, float a2, vector v1, vector v2);
vector movement_none(game_t *gs, entity_t e, float a1, float a2, vector v1, vector v2);
vector movement_pivot(game_t *gs, entity_t e, float radius, float time, vector pivot, vector v2);

#endif
