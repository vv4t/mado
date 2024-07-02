#include <gfx/gui.h>
#include <gfx/gfx_sprite.h>
#include <gfx/mesh.h>
#include <gfx/shader.h>
#include <gfx/texture.h>
#include <lib/math3d.h>
#include <lib/log.h>
#include <GL/glew.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define RECT_MAX  512

typedef struct {
  float x, y, w, h;
  float tx, ty, tw, th;
  vector color;
} rect_t;

typedef struct {
  float x, y, w, h;
} space_t;

typedef enum {
  GUI_DIV,
  GUI_BOX,
  GUI_TEXT,
  GUI_INPUTBOX,
  GUI_BUTTON
} gui_type_t;

struct gui_node_s {
  union {
    struct {
      float w, h;
      gui_node_t child;
    } div;
    struct {
      float w, h;
      int rect_ptr;
      vector color;
    } box;
    struct {
      int col, row;
      float size;
      char *text;
      int rect_ptr;
      vector color;
    } text;
    struct {
      struct {
        gui_node_t text;
        gui_node_t box;
      } inputbox;
      struct {
        gui_node_t text;
        gui_node_t box;
      } button;
      gui_node_t div;
    } sub;
  };
  bool visible;
  float x, y;
  space_t space;
  gui_invoke_t invoke;
  gui_type_t type;
  gui_node_t next;
};

struct {
  mesh_t      mesh;
  texture_t   sheet;
  shader_t    shader;
  GLuint      ubo;
  
  gui_frame_t top;
  
  gui_node_t  root;
  gui_node_t  focus;
} gui;

static gui_node_t gui_node_create(gui_type_t type);

static void rect_update(const rect_t *rect, int offset);
static int gui_push_node(int rect_num, gui_node_t node);

static void gui_mouse_press_R(gui_node_t node, float x, float y, int action);
static void gui_mouse_press_button(gui_node_t node, float x, float y, int action);

static void gui_hide_node(gui_node_t node, bool hide_next);
static void gui_update_node_R(gui_node_t node, space_t space);
static void gui_update_child(gui_node_t node, gui_node_t child, float w, float h);
static void gui_update_box(gui_node_t node);
static void gui_update_text(gui_node_t node);

void gui_init(mesh_t mesh)
{
  char define_max[256];
  snprintf(define_max, sizeof(define_max), "#define RECT_MAX %i", RECT_MAX);
  
  shaderdata_t sd = shaderdata_create();
    shaderdata_line(sd, define_max, SD_VERT);
    shaderdata_source(sd, "assets/shader/vertex/gui.vert", SD_VERT);
    shaderdata_source(sd, "assets/shader/fragment/gui.frag", SD_FRAG);
    gui.shader = shader_load(sd);
    glUniformBlockBinding(gui.shader, glGetUniformBlockIndex(gui.shader, "ubo_gui"), 2);
  shaderdata_destroy(sd);
  
  gui.sheet = texture_load_image("assets/sheet/gui.png");
  gui.mesh = mesh;
  gui.top = 0;
  gui.root = NULL;
  gui.focus = NULL;
  
  glGenBuffers(1, &gui.ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, gui.ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(rect_t) * RECT_MAX, NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 2, gui.ubo);
}

void gui_draw(const game_t *gs)
{
  shader_bind(gui.shader);
  texture_bind(gui.sheet, GL_TEXTURE_2D, 0);
  glDrawArraysInstanced(GL_TRIANGLES, gui.mesh.offset, gui.mesh.count, gui.top);
}

void gui_deinit()
{
  texture_destroy(gui.sheet);
  shader_destroy(gui.shader);
  glDeleteBuffers(1, &gui.ubo);
}

gui_frame_t gui_push(gui_node_t node)
{
  gui_frame_t frame = gui.top;
  gui.top = gui_push_node(gui.top, node);
  gui_node_update(node);
  
  if (!gui.root) {
    gui.root = node;
  } else {
    node->next = gui.root;
    gui.root = node;
  }
  
  return frame;
}

void gui_focus(gui_node_t node)
{
  gui.focus = node;
}

void gui_unfocus()
{
  gui.focus = NULL;
}

void gui_mouse_press(float x, float y, int action)
{
  gui_mouse_press_R(gui.root, x, y, action);
}

void gui_key_press(int key, int action)
{
  if (!gui.focus) {
    return;
  }
  
  gui_node_t focus = gui.focus;
  
  if (focus->type == GUI_INPUTBOX) {
    if (key == 8 && action == 1) {
      int len = strlen(focus->sub.inputbox.text->text.text);
      if (len > 0) {
        focus->sub.inputbox.text->text.text[len - 1] = 0;
        gui_node_update(focus);
      }
    }
    
    if (key == 13 && focus->invoke) {
      focus->invoke(focus, GUI_TEXT_INPUT);
    }
  }
}

void gui_text_input(const char *text)
{
  if (!gui.focus) {
    return;
  }
  
  if (gui.focus->type == GUI_INPUTBOX) {
    gui_text_printf(gui.focus->sub.inputbox.text, text);
    gui_node_update(gui.focus);
  }
}

void gui_mouse_press_R(gui_node_t node, float x, float y, int action)
{
  if (!node) {
    return;
  }
  
  switch (node->type) {
  case GUI_BUTTON:
    gui_mouse_press_button(node, x, y, action);
    break;
  case GUI_DIV:
    gui_mouse_press_R(node->div.child, x, y, action);
    break;
  case GUI_INPUTBOX:
  case GUI_BOX:
  case GUI_TEXT:
    break;
  }
  
  gui_mouse_press_R(node->next, x, y, action);
}

void gui_mouse_press_button(gui_node_t node, float x, float y, int action)
{
  gui_node_t div = node->sub.div;
  
  space_t space = (space_t) {
    .x = div->space.x + div->x * div->space.w,
    .y = div->space.y + div->y * div->space.h,
    .w = div->div.w * div->space.w,
    .h = div->div.h * div->space.h,
  };
  
  if (x > space.x && y > space.y && x < space.x + space.w && y < space.h + space.y) {
    if (action) {
      gui_box_color(node->sub.button.box, vec4(0.2, 0.2, 0.2, 0.5));
    } else {
      gui_box_color(node->sub.button.box, vec4(0.3, 0.3, 0.3, 0.5));
    }
  }
}

void gui_node_bind(gui_node_t node, gui_invoke_t invoke)
{
  node->invoke = invoke;
}

void gui_clear(gui_frame_t frame)
{
  gui.top = frame;
}

gui_node_t gui_node_create(gui_type_t type)
{
  gui_node_t node = malloc(sizeof(*node));
  node->type = type;
  node->space = (space_t) { .x = 0, .y = 0, .w = 1, .h = 1 };
  node->x = 0.0;
  node->y = 0.0;
  node->visible = true;
  node->next = NULL;
  return node;
}

gui_node_t gui_create_box()
{
  gui_node_t node = gui_node_create(GUI_BOX);
  node->box.w = 1.0;
  node->box.h = 1.0;
  node->box.color = vec4(1, 1, 1, 1);
  return node;
}

gui_node_t gui_create_text(int col, int row)
{
  gui_node_t node = gui_node_create(GUI_TEXT);
  node->text.size = 1.0;
  node->text.col = col;
  node->text.row = row;
  node->text.text = calloc(col * row, sizeof(char));
  node->text.text[0] = 0;
  node->text.color = vec4(1, 1, 1, 1);
  return node;
}

gui_node_t gui_create_div()
{
  gui_node_t node = gui_node_create(GUI_DIV);
  node->div.w = 1.0;
  node->div.h = 1.0;
  node->div.child = NULL;
  return node;
}

gui_node_t gui_create_inputbox(int max_text)
{
  gui_node_t div = gui_create_div();
    gui_div_resize(div, max_text * 5.0 / 8.0, 1.0);
  gui_node_t box = gui_create_box();
    gui_node_move(box, 0.0, 0.0);
    gui_box_resize(box, 1.0, 1.0);
    gui_box_color(box, vec4(0.3, 0.3, 0.3, 0.5));
  gui_node_t text = gui_create_text(max_text, 1);
    gui_text_color(text, vec4(1, 1, 1, 1));
  gui_div_attach(div, box);
  gui_div_attach(div, text);
  
  gui_node_t node = gui_node_create(GUI_INPUTBOX);
  node->sub.div = div;
  node->sub.inputbox.box = box;
  node->sub.inputbox.text = text;
  return node;
}

gui_node_t gui_create_button(const char *btn_text)
{
  gui_node_t div = gui_create_div();
    gui_div_resize(div, 1.0, 1.0);
  gui_node_t box = gui_create_box();
    gui_node_move(box, 0.0, 0.0);
    gui_box_resize(box, 1.0, 1.0);
    gui_box_color(box, vec4(0.3, 0.3, 0.3, 0.5));
  gui_node_t text = gui_create_text(strlen(btn_text) + 1, 1);
    gui_node_move(text, 0.02, 0.3 * 0.5);
    gui_text_resize(text, 0.7);
    gui_text_color(text, vec4(1, 1, 1, 1));
    gui_text_printf(text, "%s", btn_text);
  gui_div_attach(div, box);
  gui_div_attach(div, text);
  
  gui_node_t node = gui_node_create(GUI_BUTTON);
  node->sub.div = div;
  node->sub.button.box = box;
  node->sub.button.text = text;
  return node;
}

void gui_div_resize(gui_node_t node, float w, float h)
{
  node->div.w = w;
  node->div.h = h;
}

void gui_div_attach(gui_node_t node, gui_node_t child)
{
  if (!node->div.child) {
    node->div.child = child;
  } else {
    child->next = node->div.child;
    node->div.child = child;
  }
}

void gui_box_resize(gui_node_t node, float w, float h)
{
  node->box.w = w;
  node->box.h = h;
}

void gui_box_color(gui_node_t node, vector color)
{
  node->box.color = color;
}

void gui_text_resize(gui_node_t node, float size)
{
  node->text.size = size;
}

void gui_text_color(gui_node_t node, vector color)
{
  node->text.color = color;
}

void gui_text_printf(gui_node_t node, const char *format, ...)
{
  int len = node->text.row * node->text.col - strlen(node->text.text);
  
  if (len <= 0) {
    return;
  }
  
  va_list args;
  va_start(args, format);
  vsnprintf(&node->text.text[strlen(node->text.text)], len, format, args);
  va_end(args);
}

void gui_text_clear(gui_node_t node)
{
  node->text.text[0] = 0;
}

void gui_inputbox_clear(gui_node_t node)
{
  gui_text_clear(node->sub.inputbox.text);
}

void gui_inputbox_resize(gui_node_t node, float size)
{
  gui_div_resize(node->sub.div, node->sub.div->div.w * size, size);
}

const char *gui_inputbox_get_value(gui_node_t node)
{
  return node->sub.inputbox.text->text.text;
}

void gui_button_resize(gui_node_t node, float w, float h)
{
  gui_div_resize(node->sub.div, w, h);
}

void gui_node_move(gui_node_t node, float x, float y)
{
  node->x = x;
  node->y = y;
}

int gui_push_node(int rect_num, gui_node_t node)
{
  if (!node) {
    return rect_num;
  }
  
  rect_num = gui_push_node(rect_num, node->next);
  
  switch (node->type) {
  case GUI_BUTTON:
  case GUI_INPUTBOX:
    rect_num = gui_push_node(rect_num, node->sub.div);
    break;
  case GUI_DIV:
    rect_num = gui_push_node(rect_num, node->div.child);
    break;
  case GUI_BOX:
    node->box.rect_ptr = rect_num++;
    break;
  case GUI_TEXT:
    node->text.rect_ptr = rect_num;
    rect_num += node->text.row * node->text.col;
    break;
  }
  
  return rect_num;
}

void gui_node_update(gui_node_t node)
{
  space_t screen = (space_t) {
    .x = 0, .y = 0, .w = 1280.0 / 960.0, .h = 1
  };
  
  gui_update_node_R(node, screen);
}

void gui_update_node_R(gui_node_t node, space_t space)
{
  if (!node) {
    return;
  }
  
  gui_update_node_R(node->next, space);
  
  node->space = space;
  if (!node->visible) {
    gui_hide_node(node, false);
  } else {
    switch (node->type) {
    case GUI_BUTTON:
    case GUI_INPUTBOX:
      gui_update_child(node, node->sub.div, 1.0, 1.0);
      break;
    case GUI_DIV:
      gui_update_child(node, node->div.child, node->div.w, node->div.h);
      break;
    case GUI_BOX:
      gui_update_box(node);
      break;
    case GUI_TEXT:
      gui_update_text(node);
      break;
    }
  }
}

void gui_update_child(gui_node_t node, gui_node_t child, float w, float h)
{
  space_t new_space = (space_t) {
    .x = node->space.x + node->x * node->space.w,
    .y = node->space.y + node->y * node->space.h,
    .w = w * node->space.w,
    .h = h * node->space.h,
  };
  
  gui_update_node_R(child, new_space);
}

void gui_update_box(gui_node_t node)
{
  rect_t rect = {
    .x = node->space.x + node->x * node->space.w,
    .y = node->space.y + node->y * node->space.h,
    .w = node->box.w * node->space.w,
    .h = node->box.h * node->space.h,
    .tx = 0.0, .ty = 0.0, .tw = 1.0, .th = 1.0,
    .color = node->box.color
  };
  
  rect_update(&rect, node->box.rect_ptr);
}

void gui_update_text(gui_node_t node)
{
  int col = 0;
  int row = 0;
  
  rect_t glyph = (rect_t) {
    .x = node->space.x + node->x * node->space.w,
    .y = node->space.y + node->y * node->space.h,
    .w = 5.0 / 8.0 * node->text.size * node->space.h,
    .h = node->text.size * node->space.h,
    .tw = 5.0 / 8.0, .th = 1.0,
    .color = node->text.color
  };
  
  for (int i = 0; i < strlen(node->text.text); i++) {
    float tx = 0.0;
    float ty = 15.0;
    
    int letter = node->text.text[i];
    
    if (letter == '\n') {
      row++;
      col = 0;
    } else {
      if (letter >= ' ' && letter <= '~') {
        letter -= ' ';
        tx = (float) (letter % 16);
        ty = 15.0 - floor(letter / 16.0);
      }
      
      rect_t rect = glyph;
      rect.x += col * glyph.w;
      rect.y += row * glyph.h;
      rect.tx = tx;
      rect.ty = ty;
      rect_update(&rect, node->text.rect_ptr + i);
      col++;
    }
    
    if (col >= node->text.col) {
      row++;
      col = 0;
    }
    
    if (row >= node->text.row) {
      break;
    }
  }
  
  for (int i = strlen(node->text.text); i < node->text.row * node->text.col; i++) {
    rect_t rect = {0};
    rect_update(&rect, node->text.rect_ptr + i);
  }
}

void gui_hide_node(gui_node_t node, bool hide_next)
{
  if (!node) {
    return;
  }
  
  if (hide_next) {
    gui_hide_node(node->next, true);
  }
  
  rect_t rect = {0};
  switch (node->type) {
  case GUI_BUTTON:
  case GUI_INPUTBOX:
    gui_hide_node(node->sub.div, true);
    break;
  case GUI_DIV:
    gui_hide_node(node->div.child, true);
    break;
  case GUI_BOX:
    rect_update(&rect, node->box.rect_ptr);
    break;
  case GUI_TEXT:
    for (int i = 0; i < node->text.row * node->text.col; i++) {
      rect_update(&rect, node->text.rect_ptr + i);
    }
    break;
  }
}

void gui_node_show(gui_node_t node, bool visible)
{
  if (!node) {
    return;
  }
  
  node->visible = visible;
  switch (node->type) {
  case GUI_BUTTON:
  case GUI_INPUTBOX:
    gui_node_show(node->sub.div, visible);
    break;
  case GUI_DIV:
    gui_node_show(node->div.child, visible);
    break;
  case GUI_BOX:
  case GUI_TEXT:
    break;
  }
}

bool gui_node_is_visible(gui_node_t node)
{
  return node->visible;
}

void gui_destroy(gui_node_t node)
{
  if (!node) {
    return;
  }
  
  free(node->next);
  
  switch (node->type) {
  case GUI_BUTTON:
  case GUI_INPUTBOX:
    gui_destroy(node->sub.div);
    break;
  case GUI_DIV:
    gui_destroy(node->div.child);
    break;
  case GUI_BOX:
    break;
  case GUI_TEXT:
    free(node->text.text);
    break;
  }
  
  free(node);
}

void rect_update(const rect_t *rect, int offset)
{
  glBindBuffer(GL_UNIFORM_BUFFER, gui.ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, offset * sizeof(rect_t), sizeof(rect_t), rect);
}
