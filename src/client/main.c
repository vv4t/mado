#include <client/window.h>
#include <lib/input.h>
#include <lib/map.h>
#include <game/game.h>
#include <renderer/renderer.h>

#define WIDTH 800
#define HEIGHT 600

struct {
  game_t gs;
  input_t in;
} mado;

void update();

int main(int argc, char *argv[])
{
  window_init(WIDTH, HEIGHT, "mado");
  renderer_init();
  game_init(&mado.gs);
  mado.in = input_create();
  
  map_t map = map_load("assets/map/1.map");
  
  renderer_load_map(map);
  game_load_map(&mado.gs, map);
  
  int prev_time = window_get_time();
  int lag_time = 0;
  
  while (window_loop(mado.in)) {
    int now_time = window_get_time();
    lag_time += now_time - prev_time;
    prev_time = now_time;
    
    while (lag_time > 15) {
      lag_time -= 15;
      game_update(&mado.gs, mado.in);
    }
    
    renderer_render(&mado.gs);
    
  }
  
  renderer_deinit();
  window_deinit();
  
  return 0;
}
