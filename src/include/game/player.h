#ifndef PLAYER_H
#define PLAYER_H

#include <game/game.h>
#include <lib/input.h>

entity_t player_create(game_t *gs);
void player_update(game_t *gs, entity_t p, const input_t in);

#endif
