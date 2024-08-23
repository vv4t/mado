#ifndef USERCMD_H
#define USERCMD_H

#include <stdbool.h>

typedef struct {
  bool forward;
  bool back;
  bool left;
  bool right;
  bool rotate_left;
  bool rotate_right;
  bool attack;
  float aim_x;
  float aim_y;
} usercmd_t;

#endif
