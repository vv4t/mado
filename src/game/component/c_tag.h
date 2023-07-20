#ifndef C_TAG_H
#define C_TAG_H

#include "component.h"

typedef enum {
  TAG_PLAYER = (1 << 0),
  TAG_ENEMY  = (1 << 1)
} c_tag_t;

inline component_t c_tag_init(c_tag_t *c_tag, c_tag_t tag)
{
  *c_tag = tag;
  return COMPONENT_TAG;
}

#endif
