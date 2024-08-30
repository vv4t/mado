#include <client/client.h>
#include <game/game.h>
#include <game/player.h>
#include <game/enemy.h>
#include <gfx/gui.h>

static struct {
  gui_node_t hud;
  gui_node_t reset;
} scene1;

static void reset_handle(gui_node_t node, gui_event_t event);

static void scene1_load()
{
  gui_node_t hud = gui_create_text(32, 2);
  gui_text_resize(hud, 0.025);
  gui_node_update(hud);
  scene1.hud = hud;
  
  gui_node_t reset = gui_create_text(7, 1);
  gui_text_printf(reset, "[RESET]");
  gui_text_resize(reset, 0.025);
  gui_node_move(reset, 0.025, 0.04);
  gui_node_bind(reset, reset_handle);
  gui_node_update(reset);
  scene1.reset = reset;

  game_t *gs = client_get_game();
  game_spawn_group(gs, "main");
  game_spawn_group(gs, "fire_test");
}

static void scene1_update()
{
  game_t *gs = client_get_game();
  transform_t *pt = entity_get_component(gs, gs->player, transform);
  health_t *ph = entity_get_component(gs, gs->player, health);
  
  gui_text_clear(scene1.hud);
  gui_text_printf(scene1.hud, "HP:%i X:%.2f Y:%.2f", ph->hp, pt->position.x, pt->position.y);
  gui_node_update(scene1.hud);
}

static void scene1_destroy()
{
}

static void reset_handle(gui_node_t node, gui_event_t event)
{
  game_t *gs = client_get_game();
  
  switch (event) {
  case GUI_EV_HOVER_ENTER:
    gui_text_color(scene1.reset, vec3(0.5, 0.5, 0.5));
    break;
  case GUI_EV_HOVER_LEAVE:
    gui_text_color(scene1.reset, vec3(1.0, 1.0, 1.0));
    break;
  case GUI_EV_CLICK:
    game_reset(gs);
    game_spawn_group(gs, "main");
    game_spawn_group(gs, "fire_test");
    break;
  }
  
  gui_node_update(scene1.reset);
}

client_scene_t client_scene1 = {
  .map = "assets/map/1.map",
  .load = scene1_load,
  .update = scene1_update,
  .destroy = scene1_destroy
};
