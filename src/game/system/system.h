#ifndef SYSTEM_H
#define SYSTEM_H

#include "../edict.h"
#include "../map.h"

#define DELTA_TIME 0.015

void sys_perform_action(edict_t *edict);
void sys_animate_sprite(edict_t *edict);
void sys_physics_update(edict_t *edict, const map_t *map);

#endif
