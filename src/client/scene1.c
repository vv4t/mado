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
  gui_node_update(scene1.thing);

  game_t *gs = client_get_game();
  game_spawn_group(gs, "main");
}

static void scene1_update()
{
  game_t *gs = client_get_game();
  transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  gui_text_clear(scene1.thing);
  gui_text_printf(scene1.thing, "X:%.2f Y:%.2f", pt->position.x, pt->position.y);
  gui_node_update(scene1.thing);
}

client_scene_t client_scene1 = {
  .map = "assets/map/1.map",
  .load = scene1_load,
  .update = scene1_update
};
