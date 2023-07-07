#ifndef USERCMD_H
#define USERCMD_H

typedef struct {
  float forward;
  float side;
  float rot;
  
  float aim_x;
  float aim_y;

  bool attack;
} usercmd_t;

#endif
