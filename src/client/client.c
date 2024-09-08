#include <client/client.h>
#include <client/window.h>
#include <client/input.h>
#include <gfx/renderer.h>

#ifdef __EMSCRIPTEN__
  #include <emscripten.h>
#endif

struct {
  game_t gs;
  map_t map;
  client_scene_t scene;
} client = {
  .map = NULL,
  .scene.destroy = NULL
};

static void web_update();
static void client_update();

int main(int argc, char *argv[])
{
  window_init();
  renderer_init();
  game_init(&client.gs);
  
  client_load_scene(client_scene1);
  
  int prev_time = window_get_time();
  int lag_time = 0;  
  
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(web_update, 0, 1);
#else
  while (window_poll()) {
    if (lag_time > 0) {
      lag_time -= 15;
      client_update();
      window_swap();
    }
    
    int now_time = window_get_time();
    lag_time += now_time - prev_time;
    prev_time = now_time;
  }
  
  client.scene.destroy();
  renderer_deinit();
  window_deinit();
  map_destroy(client.map);
#endif

  return 0;
}

void web_update()
{
  window_poll();
  client_update();
  window_swap();
}

void client_update()
{
  game_update(&client.gs, input_get_usercmd());
  client.scene.update();
  renderer_render(&client.gs);
}

void client_load_scene(client_scene_t scene)
{
  if (client.scene.destroy) {
    client.scene.destroy();
  }
  
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
