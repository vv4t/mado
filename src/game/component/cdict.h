#ifndef CDICT_H
#define CDICT_H

#include "c_tag.h"
#include "c_box.h"
#include "c_actor.h"
#include "c_health.h"
#include "c_motion.h"
#include "c_sprite.h"
#include "c_bullet.h"
#include "c_ai_move.h"
#include "c_animator.h"
#include "c_transform.h"

#include "../edict.h"

typedef struct {
  c_tag_t tag[MAX_ENTITIES];
  c_box_t box[MAX_ENTITIES];
  c_actor_t actor[MAX_ENTITIES];
  c_motion_t motion[MAX_ENTITIES];
  c_health_t health[MAX_ENTITIES];
  c_sprite_t sprite[MAX_ENTITIES];
  c_bullet_t bullet[MAX_ENTITIES];
  c_ai_move_t ai_move[MAX_ENTITIES];
  c_animator_t animator[MAX_ENTITIES];
  c_transform_t transform[MAX_ENTITIES];
} cdict_t;

#endif
