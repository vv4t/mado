#ifndef CLIENT_H
#define CLIENT_H

#include <game/game.h>

typedef struct {
  void (*load)();
  void (*update)();
} client_scene_t;

game_t *client_get_game();

extern client_scene_t client_scene1;

#endif
