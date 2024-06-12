#ifndef GUI_H
#define GUI_H

#include <gfx/mesh.h>

typedef struct gui_node_s *gui_node_t;
typedef int gui_frame_t;

void gui_node_move(gui_node_t node, float w, float h);
void gui_node_color(gui_node_t node, vector color);
void gui_node_attach(gui_node_t node, gui_node_t child);
void gui_node_destroy(gui_node_t node);
gui_frame_t gui_push(const gui_node_t node);

gui_node_t gui_create_box();
void gui_box_resize(gui_node_t node, float w, float h);

gui_node_t gui_create_label(int max_text);
void gui_label_resize(gui_node_t node, float size);

void gui_init(mesh_t mesh);
void gui_draw();
void gui_deinit();

#endif
