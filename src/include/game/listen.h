#ifndef LISTEN_H
#define LISTEN_H

#define EVENT_MAX 12

struct game_s;

typedef enum {
  EV_ACT0, EV_ACT1, EV_ACT2, EV_ACT3,
  EV_ACT4, EV_ACT5, EV_ACT6, EV_ACT7,
  EV_MAP_COLLIDE
} event_type_t;

typedef struct {
  event_type_t type;
} event_t;

typedef void (*invoke_t)(struct game_s *ed, int e, event_t ev);

typedef struct {
  invoke_t  invoke;
} listen_t;

inline static listen_t create_listen()
{
  return (listen_t) {0};
}

#endif
