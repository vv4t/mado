#include <gfx/gui.h>
#include <gfx/mesh.h>
#include <gfx/shader.h>
#include <gfx/texture.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define RECT_MAX 512

typedef struct {
  float x, y, w, h;
  float tx, ty, tw, th;
  vector color;
} rect_t;

typedef enum {
  GUI_RECT,
  GUI_TEXT
} gui_node_type_t;

struct gui_node_s {
  union {
    struct {
      int offset;
      float w, h;
      vector color;
    } rect;
    struct {
      int offset;
      float size;
      int col, row;
      char *src;
      vector color;
    } text;
  };
  float x, y;
  gui_node_t next;
  gui_handle_t handle;
  gui_node_type_t type;
};

struct {
  mesh_t      mesh;
  texture_t   sheet;
  shader_t    shader;
  
  gui_node_t  root;
  vector      prev_mouse;
  
  GLuint      ubo;
  int         top;
} gui;

static gui_node_t gui_create_node();
static void gui_rect_update(const gui_node_t node);
static void gui_text_update(const gui_node_t node);
static bool gui_node_test_bound(const gui_node_t node, vector point);
static vector gui_node_bound(const gui_node_t node);
static void gui_invoke(gui_node_t node, gui_event_t event);
static void sub_rect(int offset, rect_t rect);

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
  
  gui.root = NULL;
  gui.sheet = texture_load_image("assets/sheet/gui.png");
  gui.mesh = mesh;
  
  glGenBuffers(1, &gui.ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, gui.ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(rect_t) * RECT_MAX, NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 2, gui.ubo);
}

void gui_draw()
{
  shader_bind(gui.shader);
  texture_bind(gui.sheet, GL_TEXTURE_2D, 0);
  glDrawArraysInstanced(GL_TRIANGLES, gui.mesh.offset, gui.mesh.count, gui.top);
}

void gui_deinit()
{
  shader_destroy(gui.shader);
}

gui_node_t gui_create_text(int col, int row)
{
  gui_node_t node = gui_create_node(GUI_TEXT);
  node->text.offset = gui.top;
  node->text.size = 1.0;
  node->text.col = col;
  node->text.row = row;
  node->text.src = calloc(col * row + 1, sizeof(char));
  node->text.src[0] = 0;
  node->text.color = vec4(1.0, 1.0, 1.0, 1.0);
  gui.top += col * row;
  return node;
}

void gui_text_printf(gui_node_t node, const char *format, ...)
{
  int len = node->text.row * node->text.col - strlen(node->text.src) + 1;
  
  if (len <= 0) {
    return;
  }
  
  va_list args;
  va_start(args, format);
  vsnprintf(&node->text.src[strlen(node->text.src)], len, format, args);
  va_end(args);
}

void gui_text_clear(gui_node_t node)
{
  node->text.src[0] = 0;
}

void gui_text_resize(gui_node_t node, float size)
{
  node->text.size = size;
}

void gui_text_color(gui_node_t node, vector color)
{
  node->text.color = color;
}

gui_node_t gui_create_rect()
{
  gui_node_t node = gui_create_node(GUI_RECT);
  node->rect.w = 1.0;
  node->rect.h = 1.0;
  node->rect.color = vec4(1.0, 1.0, 1.0, 1.0);
  node->rect.offset = gui.top++;
  return node;
}

void gui_rect_resize(gui_node_t node, float w, float h)
{
  node->rect.w = w;
  node->rect.h = h;
}

gui_node_t gui_create_node(gui_node_type_t type)
{
  gui_node_t node = malloc(sizeof(*node));
  node->type = type;
  node->next = gui.root;
  node->handle = NULL;
  node->x = 0.0;
  node->y = 0.0;
  gui.root = node;
  return node;
}

void gui_node_move(gui_node_t node, float x, float y)
{
  node->x = x;
  node->y = y;
}

void gui_node_bind(gui_node_t node, gui_handle_t handle)
{
  node->handle = handle;
}

void gui_mouse_move(float x, float y)
{
  vector mouse = vec2(x, y);
  gui_node_t node = gui.root;
  
  while (node) {
    bool in_prev = gui_node_test_bound(node, gui.prev_mouse);
    bool in_now = gui_node_test_bound(node, mouse);
    
    if (in_prev && !in_now) {
      gui_invoke(node, GUI_EV_HOVER_LEAVE);
    } else if (!in_prev && in_now) {
      gui_invoke(node, GUI_EV_HOVER_ENTER);
    }
    node = node->next;
  }
  
  gui.prev_mouse = vec2(x, y);
}

void gui_click()
{
  gui_node_t node = gui.root;
  while (node) {
    if (gui_node_test_bound(node, gui.prev_mouse)) {
      gui_invoke(node, GUI_EV_CLICK);
    }
    node = node->next;
  }
}

bool gui_node_test_bound(const gui_node_t node, vector point)
{
  vector bound = gui_node_bound(node);
  return (
    point.x > bound.x
    && point.y > bound.y
    && point.x < bound.z
    && point.y < bound.w
  );
}

vector gui_node_bound(const gui_node_t node)
{
  switch (node->type) {
  case GUI_RECT:
    return vec4(node->x, node->y, node->x + node->rect.w, node->y + node->rect.h);
  case GUI_TEXT:
    return vec4(
      node->x, node->y,
      node->x + node->text.col * node->text.size * 5.0 / 8.0,
      node->y + node->text.row * node->text.size
    );
  default:
    return vec4(0, 0, 0, 0);
    break;
  }
}

void gui_node_update(const gui_node_t node)
{
  switch (node->type) {
  case GUI_RECT:
    gui_rect_update(node);
    break;
  case GUI_TEXT:
    gui_text_update(node);
    break;
  default:
    break;
  }
}

void gui_rect_update(const gui_node_t node)
{
  sub_rect(node->rect.offset, (rect_t) {
    .x = node->x,
    .y = node->y,
    .w = node->rect.w,
    .h = node->rect.h,
    .tx = 0.0, .ty = 0.0, .tw = 1.0, .th = 1.0,
    .color = node->rect.color
  });
}

void gui_text_update(const gui_node_t node)
{
  rect_t glyph = (rect_t) {
    .x = node->x,
    .y = node->y,
    .w = 5.0 / 8.0 * node->text.size,
    .h = node->text.size,
    .tw = 5.0 / 8.0, .th = 1.0,
    .color = node->text.color
  };
  
  int col = 0;
  int row = 0;
  
  for (int i = 0; i < strlen(node->text.src); i++) {
    int letter = node->text.src[i];
    
    if (letter == '\n') {
      row++;
      col = 0;
    } else {
      if (letter >= ' ' && letter <= '~') {
        letter -= ' ';
        glyph.tx = (float) (letter % 16);
        glyph.ty = 15.0 - floor(letter / 16.0);
      } else {
        glyph.tx = 0.0;
        glyph.ty = 15.0;
      }
      
      glyph.x = node->x + col * glyph.w;
      glyph.y = node->y + row * glyph.h;
      sub_rect(node->text.offset + i, glyph);
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
  
  for (int i = strlen(node->text.src); i < node->text.row * node->text.col; i++) {
    rect_t empty = (rect_t) {0};
    sub_rect(node->text.offset + i, empty);
  }
}

void sub_rect(int offset, rect_t rect)
{
  glBindBuffer(GL_UNIFORM_BUFFER, gui.ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, offset * sizeof(rect_t), sizeof(rect_t), &rect);
}

static void gui_invoke(gui_node_t node, gui_event_t event)
{
  if (node->handle) {
    node->handle(node, event);
  }
}
