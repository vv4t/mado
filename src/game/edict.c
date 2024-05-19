#include <game/edict.h>
#include <lib/log.h>

void edict_init(edict_t *edict)
{
  *edict = (edict_t) {0};
  edict->num_entities = 0;
}

entity_t edict_add(edict_t *edict)
{
  entity_t entity = -1;
  
  for (int i = 0; i < edict->num_entities; i++) {
    if (edict->field[i] == 0) {
      entity = i;
      break;
    }
  }
  
  if (entity == -1) {
    entity = edict->num_entities++;
  }
  
  if (edict->num_entities >= ENTITY_MAX) {
    LOG_ERROR("edict: out of memory");
  }
  
  return entity;
}

void edict_kill(edict_t *edict, entity_t e)
{
  if (e >= ENTITY_MAX) {
    return;
  }
  
  edict->field[e] = 0;
}
