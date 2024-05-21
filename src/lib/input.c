#include <lib/input.h>
#include <stdlib.h>

#define KEY_MAX 256
#define BUTTON_MAX 8

struct input_s {
  float mouse_x;
  float mouse_y;
  int key[KEY_MAX];
  int button[BUTTON_MAX];
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

void input_mouse_down(input_t in, int button)
{
  if (button >= 0 && button < BUTTON_MAX) {
    in->button[button] = 1;
  }
}

void input_mouse_up(input_t in, int button)
{
  if (button >= 0 && button < BUTTON_MAX) {
    in->button[button] = 0;
  }
}

void input_mouse_move(input_t in, float x, float y)
{
  in->mouse_x = x * 2.0 - 1.0;
  in->mouse_y = -(y * 2.0 - 1.0);
}

int input_is_key_pressed(const input_t in, int key)
{
  return in->key[key];
}

int input_is_mouse_pressed(const input_t in, int button)
{
  return in->button[button];
}

float input_get_mouse_x(const input_t in)
{
  return in->mouse_x;
}

float input_get_mouse_y(const input_t in)
{
  return in->mouse_y;
}
