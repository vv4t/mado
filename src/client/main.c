#include <client/window.h>
#include <lib/input.h>
#include <lib/map.h>
#include <game/game.h>
#include <renderer/renderer.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
  #include <emscripten.h>
#endif

#define WIDTH 1280
#define HEIGHT 960

struct {
  game_t gs;
  input_t in;
  int prev_time;
  int lag_time;
} mado;

void web_update();
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
  
  mado.prev_time = window_get_time();
  mado.lag_time = 0;
  
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(web_update, 0, 1);
#else
  while (window_loop(mado.in)) {
    update();
  }
#endif 
  
  renderer_deinit();
  window_deinit();
  
  return 0;
}

void web_update()
{
  window_loop(mado.in);
  update();
}

void update()
{
  game_update(&mado.gs, mado.in);
  renderer_render(&mado.gs);
}
