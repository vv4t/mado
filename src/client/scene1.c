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
  scene1.thing = gui_create_text(32, 32);
  gui_text_resize(scene1.thing, 0.025);
  gui_text_printf(scene1.thing, "Play");
  gui_node_update(scene1.thing);
  
  player_spawn(client_get_game());
}

static void scene1_update()
{
  
}

client_scene_t client_scene1 = {
  .load = scene1_load,
  .update = scene1_update
};
