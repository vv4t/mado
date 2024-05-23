#include <game/game.h>
#include <game/system.h>
#include <game/player.h>
#include <game/enemy.h>
#include <stdio.h>

static void game_move_camera(game_t *gs, const input_t in);

void game_init(game_t *gs)
{
  *gs = (game_t) {0};
  edict_init(&gs->edict);
  
  edict_t *ed = &gs->edict;
  gs->player = player_create(ed);
  enemy_spawn_warrior(ed);
}

void game_update(game_t *gs, const input_t in)
{
  edict_t *ed = &gs->edict;
  
  player_update(ed, gs->player, gs->view_rotation.z, in);
  system_animate(ed);
  system_integrate(ed, gs->map);
  system_perform(ed);
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
  transform_t *pt = entity_get_component(&gs->edict, gs->player, transform);
  
  gs->view_rotation.z += input_is_key_pressed(in, 'q') * rot_speed;
  gs->view_rotation.z -= input_is_key_pressed(in, 'e') * rot_speed;
  gs->view_position = pt->position;
}
