#ifndef EDICT_H
#define EDICT_H

#define ENTITY_MAX 64
#define INVALID_ENTITY -1

#include <game/transform.h>
#include <game/sprite.h>
#include <game/rigidbody.h>

typedef int entity_t;

typedef enum {
  C_transform = 1 << 0,
  C_sprite    = 1 << 1,
  C_rigidbody = 1 << 2
} component_t;

typedef struct {
  transform_t transform[ENTITY_MAX];
  sprite_t    sprite[ENTITY_MAX];
  rigidbody_t rigidbody[ENTITY_MAX];
  component_t field[ENTITY_MAX];
  int         num_entities;
} edict_t;

void edict_init(edict_t *edict);
entity_t edict_add(edict_t *edict);

#define ENTITY_ADD_COMPONENT(edict, entity, component) \
{\
  (edict).component[entity] = create_##component();\
  (edict).field[entity] |= C_##component;\
}

#define ENTITY_GET_COMPONENT(edict, entity, component) (&(edict).component[entity])
#define ENTITY_MATCH(edict, entity, mask) (((edict).field[entity] & (mask)) != (mask))

#endif
