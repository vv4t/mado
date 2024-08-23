#include <client/input.h>

#define WIDTH 1280
#define HEIGHT 960

struct {
  usercmd_t usercmd;
} input;

void input_mouse_move(int x, int y)
{
  input.usercmd.aim_x = x / (float) WIDTH - 0.5;
  input.usercmd.aim_y = y / (float) HEIGHT - 0.5;
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
