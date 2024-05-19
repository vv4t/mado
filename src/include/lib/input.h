#ifndef INPUT_H
#define INPUT_H

#define KEY_MAX 256

typedef struct input_s *input_t;

input_t input_create();

void input_key_down(input_t in, int key);
void input_key_up(input_t in, int key);
void input_mouse_move(input_t in, int x, int y);

int input_is_key_pressed(const input_t in, int key);
int input_get_mouse_x(const input_t in);
int input_get_mouse_y(const input_t in);

#endif
