#ifndef WINDOW_H
#define WINDOW_H

#include <lib/input.h>

void window_init(int width, int height, const char *title);
int window_loop(input_t in);
void window_deinit();
int window_get_time();

#endif
