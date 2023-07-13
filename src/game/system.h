#ifndef SYSTEM_H
#define SYSTEM_H

#include "edict.h"
#include "map.h"

#define DELTA_TIME 0.015

void play_animation(animator_t *animator, const animation_t *animation);
void animate_sprite(edict_t *edict);

void box_hit_map(edict_t *edict, const map_t *map);
void box_hit_box(edict_t *edict);
void integrate_motion(edict_t *edict);
void predict_motion(edict_t *edict);

void shoot_bullet(edict_t *edict, vec2_t pos, float angle, float live_time);
void perform_act(edict_t *edict);

void orb_spawn(edict_t *edict, vec2_t pos);

#endif
