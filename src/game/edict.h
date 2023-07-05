#ifndef COMPONENT_H
#define COMPONENT_H

#define MAX_ENTITIES 8

#include "sprite.h"

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

typedef struct {
  vec2_t velocity;
  float angular_velocity;
} motion_t;

typedef struct {
  float live_time;
} bullet_t;

typedef enum {
  COMPONENT_TRANSFORM = (1 << 0),
  COMPONENT_SPRITE    = (1 << 1),
  COMPONENT_ANIMATOR  = (1 << 2),
  COMPONENT_MOTION    = (1 << 3),
  COMPONENT_BULLET    = (1 << 4)
} component_t;

typedef struct {
  component_t field[MAX_ENTITIES];
  
  transform_t transform[MAX_ENTITIES];
  motion_t motion[MAX_ENTITIES];
  sprite_t sprite[MAX_ENTITIES];
  bullet_t bullet[MAX_ENTITIES];
  animator_t animator[MAX_ENTITIES];
} edict_t;

typedef enum {
  ENTITY_INVALID = -1
} entity_t;

entity_t edict_spawn(edict_t *edict);
void edict_kill(edict_t *edict, entity_t entity);

#endif
