#include <client/client.h>
#include <game/game.h>
#include <game/player.h>
#include <game/enemy.h>
#include <gfx/gui.h>

static struct {
  gui_node_t thing;
} scene1;

static void scene1_load()
{
  scene1.thing = gui_create_rect();
  gui_rect_resize(scene1.thing, 0.2, 0.2);
  gui_node_update(scene1.thing);
  
  player_spawn(cl_get_game());
}

static void scene1_update()
{
}

cl_scene_t s_scene1 = {
  .load = scene1_load,
  .update = scene1_update
};
