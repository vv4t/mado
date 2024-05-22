#ifndef SYSTEM_H
#define SYSTEM_H

#include <game/edict.h>
#include <lib/map.h>

void system_animate(edict_t *ed);
void system_integrate(edict_t *ed, map_t map);
void system_perform(edict_t *ed);

#endif
