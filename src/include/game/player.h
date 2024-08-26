#ifndef PLAYER_H
#define PLAYER_H

#include <game/game.h>

typedef struct game_s game_t;

void player_init(game_t *gs);
void player_kill(game_t *gs);
void player_spawn(game_t *gs);
void player_update(game_t *gs, const usercmd_t *usercmd);

#endif
