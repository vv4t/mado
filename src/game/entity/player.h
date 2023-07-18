#ifndef PLAYER_H
#define PLAYER_H

#include "../game.h"
#include "../map.h"
#include "../usercmd.h"
#include "../../common/nui_math.h"

void player_init(entity_t entity, game_t *game);
void player_update(entity_t entity, game_t *game);

#endif
