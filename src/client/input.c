#include <client/input.h>
#include <gfx/gui.h>

#define WIDTH 1280
#define HEIGHT 960

struct {
  usercmd_t usercmd;
} input;

void input_mouse_move(int x, int y)
{
  float sx = x / (float) WIDTH;
  float sy = y / (float) HEIGHT;
  
  gui_mouse_move(sx, sy);
  
  input.usercmd.aim_x = sx - 0.5;
  input.usercmd.aim_y = sy - 0.5;
}

void input_key_press(int key, int action)
{
  switch (key) {
  case 'w':
    input.usercmd.forward = action;
    break;
  case 's':
    input.usercmd.back = action;
    break;
  case 'a':
    input.usercmd.left = action;
    break;
  case 'd':
    input.usercmd.right = action;
    break;
  case 'q':
    input.usercmd.rotate_left = action;
    break;
  case 'e':
    input.usercmd.rotate_right = action;
    break;
  }
}

void input_mouse_press(int button, int action)
{
  if (button == 1) {
    input.usercmd.attack = action;
  }
}

const usercmd_t *input_get_usercmd()
{
  return &input.usercmd;
}
