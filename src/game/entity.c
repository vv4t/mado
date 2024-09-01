#include <game/game.h>
#include <lib/log.h>

entity_t entity_add(game_t *gs, entname_t name)
{
  entity_t e = gs->num_entities;
  
  for (int i = 0; i < gs->num_entities; i++) {
    if (gs->entdict[i] == 0) {
      e = i;
      break;
    }
  }
  
  if (e == gs->num_entities) {
    gs->num_entities++;
  }
  
  if (e >= ENTITY_MAX) {
    LOG_ERROR("entdict: out of memory");
  }
  
  gs->entdata[e] = (entdata_t) {
    .name = name,
    .invoke = NULL
  };
  
  return e;
}

void entity_kill(game_t *gs, entity_t e)
{
  if (e >= ENTITY_MAX) {
    return;
  }
  
  gs->entdict[e] = 0;
  
  while (gs->num_entities > 0 && gs->entdict[gs->num_entities - 1] == 0) {
    gs->num_entities--;
  }
}

void entity_invoke(game_t *gs, entity_t e, event_t ev)
{
  if (e >= ENTITY_MAX) {
    return;
  }
  
  if (gs->entdata[e].invoke) {
    gs->entdata[e].invoke(gs, e, ev);
  }
}

void entity_bind(game_t *gs, entity_t e, invoke_t invoke)
{
  if (e >= ENTITY_MAX) {
    return;
  }
  
  gs->entdata[e].invoke = invoke;
}

entname_t entity_get_name(game_t *gs, entity_t e)
{
  if (e >= ENTITY_MAX) {
    return ENT_ANY;
  }
  
  return gs->entdata[e].name;
}

void *entity_get_context(game_t *gs, entity_t e, int context_size)
{
  if (e >= ENTITY_MAX) {
    return NULL;
  }
  
  if (context_size > sizeof(gs->entdata[e].context)) {
    LOG_ERROR("not enough memory: %i / %li", context_size, sizeof(gs->entdata[e].context));
  }
  
  return &gs->entdata[e].context;
}
