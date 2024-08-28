#include <client/client.h>
#include <client/window.h>
#include <client/input.h>
#include <gfx/renderer.h>

struct {
  game_t gs;
  map_t map;
  client_scene_t scene;
} client;

int main(int argc, char *argv[])
{
  window_init();
  renderer_init();
  game_init(&client.gs);
  
  client_load_scene(client_scene1);
  
  int prev_time = window_get_time();
  int lag_time = 0;  
  
  while (window_poll()) {
    if (lag_time > 0) {
      lag_time -= 15;
      game_update(&client.gs, input_get_usercmd());
      client.scene.update();
      renderer_render(&client.gs);
      window_swap();
    }
    
    int now_time = window_get_time();
    lag_time += now_time - prev_time;
    prev_time = now_time;
  }
  
  renderer_deinit();
  window_deinit();
  
  return 0;
}

void client_load_scene(client_scene_t scene)
{
  client.map = map_load(scene.map);
  game_load_map(&client.gs, client.map);
  renderer_load_map(client.map);
  client.scene = scene;
  scene.load();
}

game_t *client_get_game()
{
  return &client.gs; 
}
