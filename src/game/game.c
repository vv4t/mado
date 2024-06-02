#include <game/game.h>
#include <game/system.h>
#include <game/player.h>
#include <game/enemy.h>
#include <lib/log.h>
#include <stdio.h>

static void game_move_camera(game_t *gs, const input_t in);

void game_init(game_t *gs)
{
  *gs = (game_t) {0};
  gs->num_entities = 0;
  
  player_init(gs);
  entity_t swordboss = enemy_spawn_mr_swordboss(gs);
  actor_do(entity_get_component(gs, swordboss, actor), ACT0, 5.0);
}

void game_update(game_t *gs, const input_t in)
{
  player_update(gs, in);
  system_update_botmove(gs);
  system_animate(gs);
  system_update_bullet(gs);
  system_perform(gs);
  system_integrate(gs);
  system_collide(gs);
  system_update_health(gs);
  game_move_camera(gs, in);
  gs->time += 0.015;
}

void game_load_map(game_t *gs, map_t map)
{
  gs->map = map;
}

void game_move_camera(game_t *gs, const input_t in)
{
  float rot_speed = 0.05;
  transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  gs->view_rotation.z += input_is_key_pressed(in, 'q') * rot_speed;
  gs->view_rotation.z -= input_is_key_pressed(in, 'e') * rot_speed;
  gs->view_position = pt->position;
}

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
