#ifndef SYSTEM_H
#define SYSTEM_H

#include <game/game.h>
#include <lib/map.h>

void system_animate(game_t *gs);
void system_integrate(game_t *gs);
void system_perform(game_t *gs);
void system_update_bullet(game_t *gs);
void system_collide(game_t *gs);
void system_update_health(game_t *gs);
void system_update_botmove(game_t *gs);
void system_update_transitions(game_t *gs);

#endif
