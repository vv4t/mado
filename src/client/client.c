#include <client/client.h>
#include <client/window.h>
#include <client/input.h>
#include <gfx/renderer.h>

struct {
  game_t gs;
  client_scene_t scene;
} client;

int main(int argc, char *argv[])
{
  window_init();
  renderer_init();
  
  map_t map = map_load("assets/map/1.map");
  game_init(&client.gs, map);

  renderer_load_map(map);

  client.scene = client_scene1;
  
  client.scene.load();
  
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

game_t *client_get_game()
{
  return &client.gs; 
}
