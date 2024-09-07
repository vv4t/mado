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
  game_reset(gs);
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

static void game_move_camera(game_t *gs)
{
  transform_t *pt = entity_get_component(gs, gs->player, transform);
  gs->view_pos = pt->position;
  gs->view_rot = pt->rotation;
}

void game_load_map(game_t *gs, map_t map)
{
  gs->map = map;
}

void game_reset(game_t *gs)
{
  for (int i = 0; i < ENTITY_MAX; i++) {
    gs->entdict[i] = 0;
  }
  gs->num_entities = 0;
  player_init(gs);
}

void game_spawn_group(game_t *gs, const char *name)
{
  landmark_t lm = map_get_group(gs->map, name);
  
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
    { .match = "mr_warrior", .spawn = enemy_spawn_mr_warrior },
    { .match = "mr_shadow", .spawn = enemy_spawn_mr_shadow },
    { .match = "mr_small_warrior", .spawn = enemy_spawn_mr_small_warrior },
    { .match = "mr_small_mage", .spawn = enemy_spawn_mr_small_mage },
    { .match = "mr_fire", .spawn = enemy_spawn_mr_fire },
    { .match = "mr_phoenix", .spawn = enemy_spawn_mr_phoenix },
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
