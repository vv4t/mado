#include <client/client.h>
#include <game/game.h>
#include <game/player.h>
#include <game/enemy.h>
#include <gfx/gui.h>

static struct {
} scene1;

static void scene1_load()
{
  player_spawn(cl_get_game());
}

static void scene1_update()
{
}

cl_scene_t s_scene1 = {
  .load = scene1_load,
  .update = scene1_update
};
