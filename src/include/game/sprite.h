#ifndef SPRITE_H
#define SPRITE_H

#include <stdlib.h>
#include <lib/math3d.h>

typedef struct {
  int tx, ty, tw, th;
  int framecount;
  float frametime;
} animation_t;

typedef struct {
  float rotation;
  int stand;
  int orient;
  int tx, ty, tw, th;
  float time;
  const animation_t *repeat;
} sprite_t;

inline static sprite_t create_sprite()
{
  return (sprite_t) {
    .rotation = 0.0,
    .stand = 1,
    .orient = 1,
    .tx = 0, .ty = 0, .tw = 1, .th = 1,
    .time = 0.0,
    .repeat = NULL
  };
}

#endif
