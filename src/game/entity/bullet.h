#ifndef BULLET_H
#define BULLET_H

#include "../edict.h"

void bullet_shoot(edict_t *edict, vec2_t pos, float angle, float live_time, c_tag_t target);

#endif
