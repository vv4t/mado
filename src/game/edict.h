#ifndef COMPONENT_H
#define COMPONENT_H

#define MAX_ENTITIES 16
#define MAX_EVENT 8

#include "sprite.h"

typedef struct edict edict_t;

typedef enum {
  ENTITY_INVALID = -1
} entity_t;

typedef struct {
  vec2_t position;
  float rotation;
} transform_t;

typedef struct {
  vec2_t uv;
  float rotation;
  bool orient;
  bool stand;
} sprite_t;

typedef struct {
  const animation_t *animation;
  float time;
} animator_t;

typedef void (*xhit_t)(entity_t entity, edict_t *edict, entity_t hit);
typedef void (*xhit_map_t)(entity_t entity, edict_t *edict);

typedef struct {
  vec2_t min;
  vec2_t max;
  
  xhit_t xhit;
  xhit_map_t xhit_map;
} box_t;

typedef struct {
  vec2_t velocity;
  float angular_velocity;
  vec2_t new_pos;
} motion_t;

typedef void (*xaction_t)(entity_t entity, edict_t *edict);

typedef struct {
  xaction_t xaction;
  float time;
  float cooldown;
  int count;
  bool active;
} act_t;

typedef struct {
  act_t act[MAX_EVENT];
  int num_act;
  float angle;
} actor_t;

typedef enum {
  TAG_PLAYER = (1 << 0),
  TAG_ENEMY  = (1 << 1)
} tag_t;

typedef struct {
  tag_t target;
} bullet_t;

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

struct edict {
  component_t field[MAX_ENTITIES];
  
  tag_t tag[MAX_ENTITIES];
  box_t box[MAX_ENTITIES];
  actor_t actor[MAX_ENTITIES];
  motion_t motion[MAX_ENTITIES];
  sprite_t sprite[MAX_ENTITIES];
  bullet_t bullet[MAX_ENTITIES];
  animator_t animator[MAX_ENTITIES];
  transform_t transform[MAX_ENTITIES];
};

entity_t edict_spawn(edict_t *edict);
void edict_kill(edict_t *edict, entity_t entity);

#endif
