#ifndef C_BOX_H
#define C_BOX_H

#include "../../common/nui_math.h"

typedef int entity_t;
typedef struct edict_s edict_t;

typedef void (*xhit_t)(entity_t entity, edict_t *edict, entity_t hit);
typedef void (*xhitmap_t)(entity_t entity, edict_t *edict);

typedef struct {
  vec2_t min;
  vec2_t max;
  
  xhit_t xhit;
  xhitmap_t xhitmap;
} c_box_t;

#endif
