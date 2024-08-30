#ifndef GUI_H
#define GUI_H

#include <lib/math3d.h>
#include <gfx/mesh.h>

typedef enum {
  GUI_EV_CLICK,
  GUI_EV_HOVER_ENTER,
  GUI_EV_HOVER_LEAVE
} gui_event_t;

typedef struct gui_node_s *gui_node_t;
typedef void (*gui_handle_t)(gui_node_t node, gui_event_t event);

void gui_node_move(gui_node_t node, float x, float y);
void gui_node_update(const gui_node_t node);
void gui_node_bind(gui_node_t node, gui_handle_t handle);

gui_node_t gui_create_rect();
void gui_rect_resize(gui_node_t node, float w, float h);

gui_node_t gui_create_text(int col, int row);
void gui_text_resize(gui_node_t node, float size);
void gui_text_printf(gui_node_t node, const char *format, ...);
void gui_text_color(gui_node_t node, vector color);
void gui_text_clear(gui_node_t node);

void gui_init(mesh_t mesh);
void gui_draw();
void gui_deinit();
void gui_mouse_move(float x, float y);
void gui_click();

#endif
