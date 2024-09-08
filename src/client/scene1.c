#include <client/client.h>
#include <game/game.h>
#include <game/player.h>
#include <game/enemy.h>
#include <gfx/gui.h>

#define SPAWN_TIME 5.0

typedef enum {
  S1_WAIT,
  S1_SPAWN,
  S1_FIGHT,
  S1_WIN
} scene1_state_t;

static struct {
  gui_node_t hud;
  gui_node_t reset;
  scene1_state_t state;
  float time;
  int wave;
} scene1;

static const char *scene1_waves[] = {
  "wave1",
  "wave2",
  "wave3",
  "wave4"
};

static const int max_wave = sizeof(scene1_waves) / sizeof(scene1_waves[0]);

static void reset_handle(gui_node_t node, gui_event_t event);
static void scene1_transition(scene1_state_t state);

static void scene1_load()
{
  gui_node_t hud = gui_create_text(64, 1);
  gui_text_resize(hud, 0.025);
  gui_node_update(hud);
  scene1.hud = hud;
  
  gui_node_t reset = gui_create_text(9, 1);
  gui_text_printf(reset, "[RESTART]");
  gui_text_resize(reset, 0.025);
  gui_node_move(reset, 0.025, 0.04);
  gui_node_bind(reset, reset_handle);
  gui_node_update(reset);
  scene1.reset = reset;
  
  scene1.time = 0.0;
  scene1.wave = -1;
  
  game_t *gs = client_get_game();
  game_spawn_group(gs, "main");
  
  scene1_transition(S1_WAIT);
}

static void scene1_update()
{
  game_t *gs = client_get_game();
  health_t *ph = entity_get_component(gs, gs->player, health);
  
  gui_text_clear(scene1.hud);
  gui_text_printf(scene1.hud, "HP:%i | WAVE %i", ph->hp, scene1.wave + 1);
  if (scene1.state == S1_WAIT)
    gui_text_printf(scene1.hud, " | NEXT WAVE IN %.2fs", SPAWN_TIME - scene1.time);
  else if (scene1.state == S1_FIGHT)
    gui_text_printf(scene1.hud, " | ENEMIES:%i", gs->num_enemies);
  else if (scene1.state == S1_WIN)
    gui_text_printf(scene1.hud, " | YOU WIN!");
  gui_node_update(scene1.hud);
  
  switch (scene1.state) {
  case S1_WAIT:
    if (scene1.time > SPAWN_TIME) scene1_transition(S1_SPAWN);
    break;
  case S1_SPAWN:
    scene1_transition(S1_FIGHT);
    break;
  case S1_FIGHT:
    if (gs->num_enemies == 0 && scene1.wave + 1 == max_wave) scene1_transition(S1_WIN);
    else if (gs->num_enemies == 0) scene1_transition(S1_WAIT);
    break;
  case S1_WIN:
    break;
  }
  
  scene1.time += 0.015;
}

static void scene1_destroy()
{
  
}

static void scene1_transition(scene1_state_t state) {
  game_t *gs = client_get_game();
  
  switch (state) {
  case S1_SPAWN:
    scene1.wave++;
    game_spawn_group(gs, scene1_waves[scene1.wave]);
    break;
  default:
    break;
  }
  
  scene1.time = 0.0;
  scene1.state = state;
}

static void reset_handle(gui_node_t node, gui_event_t event)
{
  game_t *gs = client_get_game();
  
  switch (event) {
  case GUI_EV_HOVER_ENTER:
    gui_text_color(node, vec3(0.5, 0.5, 0.5));
    break;
  case GUI_EV_HOVER_LEAVE:
    gui_text_color(node, vec3(1.0, 1.0, 1.0));
    break;
  case GUI_EV_CLICK:
    game_reset(gs);
    game_spawn_group(gs, "main");
    scene1_transition(S1_WAIT);
    scene1.wave = fmax(scene1.wave - 1, -1);
    break;
  }
  
  gui_node_update(node);
}

client_scene_t client_scene1 = {
  .map = "assets/map/library.map",
  .load = scene1_load,
  .update = scene1_update,
  .destroy = scene1_destroy
};
