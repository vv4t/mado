#ifndef CLIENT_H
#define CLIENT_H

#include <game/game.h>

typedef struct {
  const char *map;
  void (*load)();
  void (*update)();
} client_scene_t;

game_t *client_get_game();
void client_load_scene(client_scene_t scene);

extern client_scene_t client_scene1;

#endif
