#ifndef GAME_H
#define GAME_H

#include <game/transform.h>
#include <game/sprite.h>
#include <game/rigidbody.h>
#include <game/actor.h>
#include <game/listen.h>
#include <lib/input.h>
#include <lib/map.h>

#define ENTITY_MAX 32

typedef int entity_t;

typedef enum {
  C_transform = 1 << 0,
  C_sprite    = 1 << 1,
  C_rigidbody = 1 << 2,
  C_actor     = 1 << 3,
  C_listen    = 1 << 4
} component_t;

typedef struct game_s {
  transform_t transform[ENTITY_MAX];
  sprite_t    sprite[ENTITY_MAX];
  rigidbody_t rigidbody[ENTITY_MAX];
  actor_t     actor[ENTITY_MAX];
  listen_t    listen[ENTITY_MAX];
  component_t edict[ENTITY_MAX];
  int         num_entities;
  
  map_t       map;
  entity_t    player;
  
  vector      view_position;
  vector      view_rotation;
  
  float       time;
} game_t;

void game_init(game_t *gs);
void game_load_map(game_t *gs, map_t map);
void game_update(game_t *gs, const input_t in);

entity_t  entity_add(game_t *gs);
void      entity_kill(game_t *gs, entity_t e);

#define entity_add_component(gs, e, component) \
{\
  (gs)->component[e] = create_##component();\
  (gs)->edict[e] |= C_##component;\
}

#define entity_get_component(gs, e, component) (&(gs)->component[e])
#define entity_match(gs, e, mask) (((gs)->edict[e] & (mask)) == (mask))

#endif
