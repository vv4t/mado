#ifndef USERCMD_H
#define USERCMD_H

typedef struct {
  float forward;
  float side;
  float rot;
  
  int relative_cursor_x;
  int relative_cursor_y;

  bool mouse_down;
} usercmd_t;

#endif
