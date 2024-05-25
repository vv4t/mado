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
  enemy_spawn_scytheman(gs);
}

void game_update(game_t *gs, const input_t in)
{
  player_update(gs, in);
  system_animate(gs);
  system_integrate(gs);
  system_perform(gs);
  system_update_bullet(gs);
  system_collide(gs);
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

entity_t entity_add(game_t *gs)
{
  for (int i = 0; i < gs->num_entities; i++) {
    if (gs->edict[i] == 0) {
      return i;
    }
  }
  
  if (gs->num_entities >= ENTITY_MAX) {
    LOG_ERROR("edict: out of memory");
  }
  
  return gs->num_entities++;
}

void entity_kill(game_t *gs, entity_t e)
{
  if (e >= ENTITY_MAX) {
    return;
  }
  
  gs->edict[e] = 0;
}

void entity_invoke(game_t *gs, entity_t e, event_t ev)
{
  if (gs->invoke) {
    gs->invoke[e](gs, e, ev);
  }
}

void entity_bind(game_t *gs, entity_t e, invoke_t invoke)
{
  gs->invoke[e] = invoke;
}
