#ifndef C_BOX_H
#define C_BOX_H

#include "../edict.h"

typedef struct game_s game_t;

typedef void (*xhit_t)(entity_t entity, game_t *game, entity_t hit);
typedef void (*xhitmap_t)(entity_t entity, game_t *game);

typedef struct {
  vec2_t min;
  vec2_t max;
  
  xhit_t xhit;
  xhitmap_t xhitmap;
} c_box_t;

#endif
