#include <client/window.h>
#include <lib/input.h>
#include <lib/map.h>
#include <game/game.h>
#include <gfx/gfx.h>
#include <gfx/gui.h>
#include <stdio.h>

#define WIDTH 1280
#define HEIGHT 960

struct {
  game_t gs;
  input_t in;
} mado;

void update();

int main(int argc, char *argv[])
{
  window_init(WIDTH, HEIGHT, "mado");
  gfx_init();
  game_init(&mado.gs);
  mado.in = input_create();
  
  gui_node_t box = gui_create_box();
    gui_node_move(box, 0.01565, 0.01565);
    gui_node_color(box, vec4(1, 0, 0, 0.5));
    gui_box_resize(box, 0.8, 0.8);
  gui_node_t text = gui_create_text(12, 4);
    gui_node_color(text, vec4(1, 1, 1, 1));
    gui_text_resize(text, 0.01565);
    gui_text_printf(text, "there is alot of text that you can put");
  gui_node_attach(box, text);
  
  gui_push(box);
  
  map_t map = map_load("assets/map/1.map");
  
  gfx_load_map(map);
  game_load_map(&mado.gs, map);
  
  int prev_time = window_get_time();
  int lag_time = 0;
  
  while (window_loop(mado.in)) {
    if (lag_time > 15) {
      lag_time -= 15;
      game_update(&mado.gs, mado.in);
      gfx_render(&mado.gs);
    }
    
    int now_time = window_get_time();
    lag_time += now_time - prev_time;
    prev_time = now_time;
  }
  
  gfx_deinit();
  window_deinit();
  
  return 0;
}
