#include <game/game.h>
#include <game/system.h>
#include <game/player.h>
#include <game/enemy.h>
#include <lib/log.h>
#include <stdio.h>

static void game_move_camera(game_t *gs, const usercmd_t *usercmd);

void game_init(game_t *gs)
{
  *gs = (game_t) {0};
  player_init(gs);
}

void game_update(game_t *gs, const usercmd_t *usercmd)
{
  player_update(gs, usercmd);
  system_update_botmove(gs);
  system_animate(gs);
  system_update_bullet(gs);
  system_perform(gs);
  system_integrate(gs);
  system_collide(gs);
  system_update_health(gs);
  game_move_camera(gs, usercmd);
  gs->time += 0.015;
}

void game_load_map(game_t *gs, map_t map)
{
  gs->map = map;
}

void game_move_camera(game_t *gs, const usercmd_t *usercmd)
{
  float rot_speed = 0.05;
  transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  gs->view_rot.z += usercmd->rotate_left * rot_speed;
  gs->view_rot.z -= usercmd->rotate_right * rot_speed;
  gs->view_pos = pt->position;
}
