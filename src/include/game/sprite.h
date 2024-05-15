#ifndef SPRITEINSTANCE_H
#define SPRITEINSTANCE_H

#include <math3d.h>

typedef enum {
  SPR_PLAYER
} spritename_t;

typedef struct {
  spritename_t name;
  int stand;
  int orient;
} sprite_t;

inline static sprite_t create_sprite()
{
  return (sprite_t) {
    .name = SPR_PLAYER,
    .stand = 1,
    .orient = 1
  };
}

#endif
