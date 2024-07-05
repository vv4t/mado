#include <client/client.h>
#include <game/game.h>
#include <game/player.h>
#include <gfx/gui.h>

static struct {
  gui_node_t debug_info;
} scene1;

static void spawn_click(gui_node_t node, gui_event_t event);
static void kill_click(gui_node_t node, gui_event_t event);

static void scene1_load()
{
  gui_node_t debug_info = gui_create_text(32, 4);
    gui_text_resize(debug_info, 8.0 / 480.0);
  gui_push(debug_info);
  
  gui_node_t spawn_btn = gui_create_button("spawn");
    gui_node_move(spawn_btn, 0.01, 0.125);
    gui_button_resize(spawn_btn, 0.2, 0.03);
    gui_node_bind(spawn_btn, spawn_click);
  gui_push(spawn_btn);
  
  gui_node_t kill_btn = gui_create_button("kill");
    gui_node_move(kill_btn, 0.01, 0.125 + 0.04);
    gui_button_resize(kill_btn, 0.2, 0.03);
    gui_node_bind(kill_btn, kill_click);
  gui_push(kill_btn);
  
  // player_spawn();
  
  scene1.debug_info = debug_info;
}

static void scene1_update()
{
  game_t *gs = cl_get_game();
  
  transform_t *pt = entity_get_component(gs, gs->player, transform);
  gui_text_clear(scene1.debug_info);
  gui_text_printf(scene1.debug_info, "x:%f y:%f\n", pt->position.x, pt->position.y);
  gui_node_update(scene1.debug_info);
}

cl_scene_t s_scene1 = {
  .load = scene1_load,
  .update = scene1_update
};

static void spawn_click(gui_node_t node, gui_event_t event)
{
  player_spawn(cl_get_game());
}

static void kill_click(gui_node_t node, gui_event_t event)
{
  player_kill(cl_get_game());
}

/*
void console_invoke(gui_node_t node, gui_event_t event);

void scene1_init()
{
  gui_node_t console = gui_create_inputbox(64);
    gui_node_bind(console, console_invoke);
    gui_node_move(console, 0.0, 1.0 - 8.0 / 480.0);
    gui_inputbox_resize(console, 8.0 / 480.0);
    gui_node_show(console, false);
  gui_push(console);
  
  if (action == 1) {
    if (!gui_node_is_visible(client.console) && key == '`') {
      gui_node_show(client.console, true);
      gui_node_update(client.console);
      gui_focus(client.console);
      SDL_StopTextInput();
      SDL_StartTextInput();
    }
  }
  
}

void scene1_update()
{
}

void console_invoke(gui_node_t node, gui_event_t event)
{
  if (event == GUI_TEXT_INPUT) {
    printf("%s\n", gui_inputbox_get_value(node));
    
    gui_inputbox_clear(node);
    gui_node_show(node, false);
    gui_node_update(node);
    
    gui_unfocus();
  }
}
*/
