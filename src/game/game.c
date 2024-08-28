#include <game/game.h>
#include <game/system.h>
#include <game/player.h>
#include <game/enemy.h>
#include <lib/log.h>
#include <stdio.h>
#include <string.h>

static void game_move_camera(game_t *gs);
static void game_spawn_landmark(game_t *gs, landmark_t lm);

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
  game_spawn_group(gs);
}

void game_spawn_group(game_t *gs)
{
  landmark_t lm = map_get_landmarks(gs->map);
  
  while (lm) {
    game_spawn_landmark(gs, lm);
    lm = landmark_next(lm);
  }
}

void game_spawn_landmark(game_t *gs, landmark_t lm)
{
  static const struct {
    const char *match;
    void (*spawn)(game_t *gs, vector position);
  } spawn_table[] = {
    { .match = "player", .spawn = player_spawn },
    { .match = "mr_warrior", .spawn = enemy_spawn_mr_warrior }
  };
  
  int num_spawn_table = sizeof(spawn_table) / sizeof(spawn_table[0]);
  
  for (int i = 0; i < num_spawn_table; i++) {
    const char *match = spawn_table[i].match;
    if (strncmp(match, landmark_get_name(lm), strlen(match)) == 0) {
      spawn_table[i].spawn(gs, landmark_get_position(lm));
      return;
    }
  }
  LOG_INFO("unknown landmark: %s", landmark_get_name(lm));
}
