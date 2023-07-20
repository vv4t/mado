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

typedef enum {
  COMPONENT_TRANSFORM = (1 << 0),
  COMPONENT_SPRITE    = (1 << 1),
  COMPONENT_ANIMATOR  = (1 << 2),
  COMPONENT_MOTION    = (1 << 3),
  COMPONENT_BULLET    = (1 << 4),
  COMPONENT_ACTOR     = (1 << 5),
  COMPONENT_BOX       = (1 << 6),
  COMPONENT_TAG       = (1 << 7),
  COMPONENT_HEALTH    = (1 << 8),
  COMPONENT_AI_MOVE   = (1 << 9)
} component_t;

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
