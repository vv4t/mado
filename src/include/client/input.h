#ifndef INPUT_H
#define INPUT_H

#include <game/usercmd.h>

void input_key_press(int key, int action);
void input_mouse_press(int button, int action);
void input_mouse_move(int x, int y);
const usercmd_t *input_get_usercmd();

#endif
