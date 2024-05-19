#include <client/window.h>
#include <lib/input.h>
#include <renderer/renderer.h>
#include <game/game.h>

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
  
  while (window_loop(mado.in)) update();
  
  renderer_deinit();
  window_deinit();
  
  return 0;
}

void update()
{
  game_update(&mado.gs, mado.in);
  renderer_render(&mado.gs);
}

