#include <client/client.h>
#include <game/game.h>
#include <game/player.h>
#include <game/enemy.h>
#include <gfx/gui.h>

static struct {
  gui_node_t info;
} scene1;

static void scene1_load()
{
  scene1.info = gui_create_text(32, 32);
  gui_text_resize(scene1.info, 0.025);
  gui_node_update(scene1.info);
  
  player_spawn(client_get_game());
  enemy_spawn_mr_fire(client_get_game(), vec2(32.0, 32.0));
}

static void scene1_update()
{
  game_t *gs = client_get_game();
  transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  gui_text_clear(scene1.info);
  gui_text_printf(scene1.info, "X:%.2f Y:%.2f", pt->position.x, pt->position.y);
  gui_node_update(scene1.info);
}

client_scene_t client_scene1 = {
  .map = "assets/map/1.map",
  .load = scene1_load,
  .update = scene1_update
};
