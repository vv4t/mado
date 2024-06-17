#ifndef PLAYER_H
#define PLAYER_H

#include <game/game.h>

struct game_s;

void player_init(struct game_s *gs);
void player_update(struct game_s *gs, const usercmd_t *usercmd);

#endif
