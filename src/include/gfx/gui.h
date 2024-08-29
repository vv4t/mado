#ifndef GUI_H
#define GUI_H

#include <gfx/mesh.h>

typedef struct gui_node_s *gui_node_t;

void gui_node_move(gui_node_t node, float x, float y);
void gui_node_update(const gui_node_t node);

gui_node_t gui_create_rect();
void gui_rect_resize(gui_node_t node, float w, float h);

gui_node_t gui_create_text(int col, int row);
void gui_text_resize(gui_node_t node, float size);
void gui_text_printf(gui_node_t node, const char *format, ...);
void gui_text_clear(gui_node_t node);

void gui_init(mesh_t mesh);
void gui_draw();
void gui_deinit();
void gui_mouse_move(float x, float y);

#endif
