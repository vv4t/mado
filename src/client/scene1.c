#include <client/client.h>
#include <game/game.h>
#include <game/player.h>
#include <game/enemy.h>
#include <gfx/gui.h>

static struct {
  gui_node_t hud;
  gui_node_t thing;
} scene1;

static void hud_handle(gui_node_t node, gui_event_t event);

static void scene1_load()
{
  scene1.hud = gui_create_text(32, 2);
  gui_node_bind(scene1.hud, hud_handle);
  gui_text_resize(scene1.hud, 0.025);
  gui_node_update(scene1.hud);

  game_t *gs = client_get_game();
  game_spawn_group(gs, "main");
}

static void scene1_update()
{
  game_t *gs = client_get_game();
  transform_t *pt = entity_get_component(gs, gs->player, transform);
  health_t *ph = entity_get_component(gs, gs->player, health);
  
  gui_text_clear(scene1.hud);
  gui_text_printf(scene1.hud, "HP:%02i X:%.2f Y:%.2f", ph->hp, pt->position.x, pt->position.y);
  gui_node_update(scene1.hud);
}

static void scene1_destroy()
{
}

static void hud_handle(gui_node_t node, gui_event_t event)
{
}

client_scene_t client_scene1 = {
  .map = "assets/map/1.map",
  .load = scene1_load,
  .update = scene1_update,
  .destroy = scene1_destroy
};
