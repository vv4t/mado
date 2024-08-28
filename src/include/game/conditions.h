#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <stdbool.h>
#include <game/game.h>

typedef int entity_t;

condition_t cond_greater_distance(entity_t other, float dist);
condition_t cond_lesser_distance(entity_t other, float dist);
condition_t cond_lesser_hp_flat(float hp);
condition_t cond_lesser_hp_percent(float hp);
condition_t cond_time_elapsed(float delay);

#endif