#ifndef SPRITEINSTANCE_H
#define SPRITEINSTANCE_H

#include <math3d.h>

typedef struct {
  int tx, ty, tw, th;
  int stand;
  int orient;
} sprite_t;

inline static sprite_t create_sprite()
{
  return (sprite_t) {
    .tx = 0, .ty = 0,
    .tw = 1, .th = 1,
    .stand = 1,
    .orient = 1
  };
}

#endif
