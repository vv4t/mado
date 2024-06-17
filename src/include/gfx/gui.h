#ifndef GUI_H
#define GUI_H

#include <gfx/mesh.h>

typedef struct gui_node_s *gui_node_t;
typedef int gui_frame_t;

typedef enum {
  GUI_CLICK,
  GUI_TEXT_INPUT
} gui_event_t;

typedef void (*gui_invoke_t)(gui_node_t node, gui_event_t event);

void gui_node_move(gui_node_t node, float x, float y);
void gui_node_destroy(gui_node_t node);
void gui_node_update(gui_node_t node);
void gui_node_bind(gui_node_t node, gui_invoke_t invoke);
void gui_node_hide(gui_node_t node);
gui_frame_t gui_push(gui_node_t node);

gui_node_t gui_create_box();
void gui_box_resize(gui_node_t node, float w, float h);
void gui_box_color(gui_node_t node, vector color);

gui_node_t gui_create_text(int col, int row);
void gui_text_printf(gui_node_t node, const char *format);
void gui_text_resize(gui_node_t node, float size);
void gui_text_color(gui_node_t node, vector color);

gui_node_t gui_create_div();
void gui_div_attach(gui_node_t node, gui_node_t child);
void gui_div_resize(gui_node_t node, float w, float h);

gui_node_t gui_create_inputbox(int max_text);
void gui_inputbox_resize(gui_node_t node, float size);
const char *gui_inputbox_get_value(gui_node_t node);
// void gui_inputbox_text_color(gui_node_t node, vector color);
// void gui_inputbox_box_color(gui_node_t node, vector color);
// void gui_inputbox_focus(gui_node_t node);

void gui_focus(gui_node_t node);
void gui_unfocus();
void gui_text_input(const char *text);
void gui_key_press(int key);
void gui_click(float x, float y);

void gui_init(mesh_t mesh);
void gui_draw();
void gui_deinit();

#endif
