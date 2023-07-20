#ifndef COMPONENT_H
#define COMPONENT_H

typedef enum {
  COMPONENT_TRANSFORM = (1 << 0),
  COMPONENT_SPRITE    = (1 << 1),
  COMPONENT_ANIMATOR  = (1 << 2),
  COMPONENT_MOTION    = (1 << 3),
  COMPONENT_BULLET    = (1 << 4),
  COMPONENT_ACTOR     = (1 << 5),
  COMPONENT_BOX       = (1 << 6),
  COMPONENT_TAG       = (1 << 7),
  COMPONENT_HEALTH    = (1 << 8),
  COMPONENT_AI_MOVE   = (1 << 9)
} component_t;

#endif
