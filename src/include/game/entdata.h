#ifndef ENTDATA_H
#define ENTDATA_H

struct game_s;

typedef enum {
  EV_ACT,
  EV_ACT0, EV_ACT1, EV_ACT2, EV_ACT3,
  EV_ACT4, EV_ACT5, EV_ACT6, EV_ACT7,
  EV_MAP_COLLIDE,
  EV_ENTITY_COLLIDE
} event_type_t;

typedef struct {
  union {
    struct { int name; } act;
    struct { int e; } entcol;
  };
  event_type_t type;
} event_t;

typedef void (*invoke_t)(struct game_s *gs, int e, event_t ev);

typedef enum {
  ENT_ANY,
  ENT_PLAYER,
  ENT_ENEMY,
  ENT_BULLET
} entname_t;

typedef struct {
  entname_t name;
  invoke_t  invoke;
} entdata_t;

#endif
