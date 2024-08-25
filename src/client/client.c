#include <client/client.h>
#include <client/window.h>
#include <client/input.h>
#include <gfx/renderer.h>

struct {
  game_t gs;
  map_t map;
  client_scene_t scene;
} client = {
  .map = NULL,
  .scene = {
    .map = NULL,
    .load = NULL,
    .update = NULL
  }
};

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
  client.scene = scene;
  client_load_map(scene.map);
  client.scene.load();
}

void client_load_map(const char *map)
{
  if (client.map) {
    map_destroy(client.map);
  }
  
  client.map = map_load(map);
  renderer_load_map(client.map);
  game_map_load(&client.gs, client.map);
}

game_t *client_get_game()
{
  return &client.gs; 
}
