#ifndef PLAYER_H
#define PLAYER_H

#include <game/edict.h>
#include <lib/input.h>

entity_t player_create(edict_t *ed);
void player_update(edict_t *ed, entity_t p, float rot_z, const input_t in);

#endif
