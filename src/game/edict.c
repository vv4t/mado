#include "edict.h"

entity_t edict_spawn(edict_t *edict)
{
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (edict->field[i] == 0)
      return i;
  }
  
  return ENTITY_INVALID;
}

void edict_kill(edict_t *edict, entity_t entity)
{
  edict->field[entity] = 0;
}
