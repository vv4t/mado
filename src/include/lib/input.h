#ifndef INPUT_H
#define INPUT_H

typedef struct input_s *input_t;

input_t input_create();

void input_mouse_down(input_t in, int button);
void input_mouse_up(input_t in, int button);
void input_key_down(input_t in, int key);
void input_key_up(input_t in, int key);
void input_mouse_move(input_t in, float x, float y);

int input_is_key_pressed(const input_t in, int key);
int input_is_mouse_pressed(const input_t in, int button);
float input_get_mouse_x(const input_t in);
float input_get_mouse_y(const input_t in);

#endif
