#ifndef SYSTEM_H
#define SYSTEM_H

#include "../game.h"
#include "../map.h"

#define DELTA_TIME 0.015

void sys_perform_action(game_t *game);
void sys_animate_sprite(game_t *game);
void sys_physics_update(game_t *game);

#endif
