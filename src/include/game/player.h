#ifndef PLAYER_H
#define PLAYER_H

#include <game/game.h>
#include <lib/input.h>

struct game_s;

void player_init(struct game_s *gs);
void player_update(struct game_s *gs, int p, float rot_z, const input_t in);

#endif
