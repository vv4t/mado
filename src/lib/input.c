#include <lib/input.h>
#include <stdlib.h>

struct input_s {
  int mouse_x;
  int mouse_y;
  int key[KEY_MAX];
};

input_t input_create()
{
  input_t in = malloc(sizeof(*in));
  *in = (struct input_s) {0};
  return in;
}

void input_key_down(input_t in, int key)
{
  if (key >= 0 && key < KEY_MAX) {
    in->key[key] = 1;
  }
}

void input_key_up(input_t in, int key)
{
  if (key >= 0 && key < KEY_MAX) {
    in->key[key] = 0;
  }
}

void input_mouse_move(input_t in, int x, int y)
{
  in->mouse_x += x;
  in->mouse_y += y;
}

int input_is_key_pressed(const input_t in, int key)
{
  return in->key[key];
}

int input_get_mouse_x(const input_t in)
{
  return in->mouse_x;
}

int input_get_mouse_y(const input_t in)
{
  return in->mouse_y;
}
