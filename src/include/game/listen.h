#ifndef LISTEN_H
#define LISTEN_H

#define EVENT_MAX 12

typedef enum {
  EV_ACT
} event_type_t;

typedef struct {
  union {
    struct {
      int id;
    } act;
  };
  event_type_t type;
} event_t;

typedef void (*invoke_t)(struct game_s *gs, int e, event_t ev);

typedef struct {
  invoke_t  invoke;
} listen_t;

inline static listen_t create_listen()
{
  return (listen_t) {0};
}

#endif
