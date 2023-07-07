#ifndef PLAYER_H
#define PLAYER_H

#include "edict.h"
#include "map.h"
#include "usercmd.h"
#include "../common/nui_math.h"

typedef struct {
  entity_t entity;
  
  float shoot_cooldown;
} player_t;

void player_init(player_t *player, edict_t *edict);
void player_update(player_t *player, edict_t *edict, const usercmd_t *usercmd);

#endif
