#ifndef COMPONENT_H
#define COMPONENT_H

#define MAX_ENTITIES 16
#define ENTITY_INVALID -1

typedef int entity_t;

typedef struct edict_s {
  int field[MAX_ENTITIES];
} edict_t;

entity_t edict_spawn(edict_t *edict);
void edict_kill(edict_t *edict, entity_t entity);

#endif
