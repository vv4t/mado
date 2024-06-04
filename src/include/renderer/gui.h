#ifndef GUI_H
#define GUI_H

#include <game/game.h>
#include <renderer/mesh.h>

void gui_init(mesh_t mesh);
void gui_draw(const game_t *gs);
void gui_deinit();

#endif
