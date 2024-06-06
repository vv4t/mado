#ifndef GUI_H
#define GUI_H

#include <game/game.h>
#include <renderer/mesh.h>

void GUI_init(mesh_t mesh);
void GUI_draw(const game_t *gs);
void GUI_deinit();

#endif
