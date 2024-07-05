#ifndef CLIENT_H
#define CLIENT_H

#include <game/game.h>

typedef struct {
  void (*load)();
  void (*update)();
} cl_scene_t;

void cl_init();
int cl_update();
void cl_deinit();

game_t *cl_get_game();
void cl_scene_load(const cl_scene_t *scene);

#endif
