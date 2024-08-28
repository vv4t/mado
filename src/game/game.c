#include <game/game.h>
#include <game/system.h>
#include <game/player.h>
#include <game/enemy.h>
#include <lib/log.h>
#include <stdio.h>

static void game_move_camera(game_t *gs);

void game_init(game_t *gs)
{
  *gs = (game_t) {0};
  player_init(gs);
}

void game_update(game_t *gs, const usercmd_t *usercmd)
{
  player_update(gs, usercmd);
  system_update_npcmove(gs);
  system_update_transitions(gs);
  system_animate(gs);
  system_update_bullet(gs);
  system_perform(gs);
  system_integrate(gs);
  system_collide(gs);
  system_update_health(gs);
  game_move_camera(gs);
  gs->time += 0.015;
}

void game_move_camera(game_t *gs)
{
  transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  gs->view_rot.z = pt->rotation.z;
  gs->view_pos = pt->position;
}

void game_load_map(game_t *gs, map_t map)
{
  gs->map = map;
}
