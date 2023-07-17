#ifndef PLAYER_H
#define PLAYER_H

#include "../edict.h"
#include "../map.h"
#include "../usercmd.h"
#include "../../common/nui_math.h"

void player_init(entity_t entity, edict_t *edict);
void player_update(entity_t entity, edict_t *edict, const usercmd_t *usercmd);

#endif
