#ifndef EDICT_H
#define EDICT_H

#define ENTITY_MAX 64

#include <game/transform.h>
#include <game/sprite.h>
#include <game/rigidbody.h>
#include <game/actor.h>
#include <game/listen.h>

typedef int entity_t;

typedef enum {
  C_transform = 1 << 0,
  C_sprite    = 1 << 1,
  C_rigidbody = 1 << 2,
  C_actor     = 1 << 3,
  C_listen    = 1 << 4
} component_t;

typedef struct edict_s {
  transform_t transform[ENTITY_MAX];
  sprite_t    sprite[ENTITY_MAX];
  rigidbody_t rigidbody[ENTITY_MAX];
  actor_t     actor[ENTITY_MAX];
  listen_t    listen[ENTITY_MAX];
  component_t field[ENTITY_MAX];
  int         num_entities;
} edict_t;

void edict_init(edict_t *edict);
entity_t edict_add(edict_t *edict);
void edict_kill(edict_t *edict, entity_t e);

#define entity_add_component(ed, entity, component) \
{\
  (ed)->component[entity] = create_##component();\
  (ed)->field[entity] |= C_##component;\
}

#define entity_get_component(ed, entity, component) (&(ed)->component[entity])
#define entity_match(ed, entity, mask) (((ed)->field[entity] & (mask)) == (mask))

#endif
