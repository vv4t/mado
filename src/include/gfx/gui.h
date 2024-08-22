#ifndef GUI_H
#define GUI_H

#include <gfx/mesh.h>

typedef struct gui_node_s *gui_node_t;

void gui_node_move(gui_node_t node, float x, float y);
void gui_node_update(const gui_node_t node);

gui_node_t gui_create_rect();
void gui_rect_resize(gui_node_t node, float w, float h);

void gui_init(mesh_t mesh);
void gui_draw();
void gui_deinit();

#endif
