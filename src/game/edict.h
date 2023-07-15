#ifndef COMPONENT_H
#define COMPONENT_H

#include "component/c_tag.h"
#include "component/c_box.h"
#include "component/c_actor.h"
#include "component/c_motion.h"
#include "component/c_sprite.h"
#include "component/c_bullet.h"
#include "component/c_animator.h"
#include "component/c_transform.h"

#define MAX_ENTITIES 16

typedef int entity_t;

#define ENTITY_INVALID -1

typedef enum {
  COMPONENT_TRANSFORM = (1 << 0),
  COMPONENT_SPRITE    = (1 << 1),
  COMPONENT_ANIMATOR  = (1 << 2),
  COMPONENT_MOTION    = (1 << 3),
  COMPONENT_BULLET    = (1 << 4),
  COMPONENT_ACTOR     = (1 << 5),
  COMPONENT_BOX       = (1 << 6),
  COMPONENT_TAG       = (1 << 7)
} component_t;

typedef struct edict_s {
  int field[MAX_ENTITIES];
  
  c_tag_t tag[MAX_ENTITIES];
  c_box_t box[MAX_ENTITIES];
  c_actor_t actor[MAX_ENTITIES];
  c_motion_t motion[MAX_ENTITIES];
  c_sprite_t sprite[MAX_ENTITIES];
  c_bullet_t bullet[MAX_ENTITIES];
  c_animator_t animator[MAX_ENTITIES];
  c_transform_t transform[MAX_ENTITIES];
} edict_t;

entity_t edict_spawn(edict_t *edict);
void edict_kill(edict_t *edict, entity_t entity);

#endif
