#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <stdbool.h>
#include <game/game.h>

typedef int entity_t;

bool cond_greater_distance(game_t *gs, entity_t e, entity_t arg1, float arg2);
bool cond_lesser_distance(game_t *gs, entity_t e, entity_t arg1, float arg2);
bool cond_lesser_hp_flat(game_t *gs, entity_t e, entity_t arg1, float arg2);
bool cond_lesser_hp_percent(game_t *gs, entity_t e, entity_t arg1, float arg2);

#endif