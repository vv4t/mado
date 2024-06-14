#include <gfx/gui.h>
#include <gfx/gfx_sprite.h>
#include <gfx/mesh.h>
#include <gfx/shader.h>
#include <gfx/texture.h>
#include <lib/math3d.h>
#include <lib/log.h>
#include <GL/glew.h>
#include <string.h>
#include <stdio.h>

#define RECT_MAX  256

typedef struct {
  float x, y, w, h;
  float tx, ty, tw, th;
  vector color;
} rect_t;

typedef struct {
  float x, y, w, h;
} space_t;

typedef enum {
  GUI_BOX,
  GUI_LABEL
} gui_type_t;

struct gui_node_s {
  union {
    struct {
      float w, h;
      int rect_ptr;
    } box;
    struct {
      int col, row;
      float size;
      char *text;
      int rect_ptr;
    } text;
  };
  float x, y;
  vector color;
  space_t space;
  gui_type_t type;
  gui_node_t next;
  gui_node_t child;
};

struct {
  mesh_t      mesh;
  texture_t   sheet;
  shader_t    shader;
  GLuint      ubo;
  gui_frame_t top;
} gui;

static gui_node_t gui_node_create(gui_type_t type);

static void rect_update(const rect_t *rect, int offset);
static int gui_push_node(int rect_num, gui_node_t node, space_t space);
static int gui_push_box(int rect_num, gui_node_t node, space_t space);
static int gui_push_text(int rect_num, gui_node_t node, space_t space);

static void gui_update_box(const gui_node_t node);
static void gui_update_text(const gui_node_t node);

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

gui_frame_t gui_push(const gui_node_t node)
{
  space_t screen = (space_t) {
    .x = 0, .y = 0, .w = 1, .h = 1
  };
  
  gui_frame_t frame = gui.top;
  gui.top = gui_push_node(gui.top, node, screen);
  
  return frame;
}

void gui_node_color(gui_node_t node, vector color)
{
  node->color = color;
}

void gui_reset(gui_frame_t frame)
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
  node->color = vec4(1, 1, 1, 1);
  node->next = NULL;
  node->child = NULL;
  return node;
}

gui_node_t gui_create_box()
{
  gui_node_t node = gui_node_create(GUI_BOX);
  node->box.w = 1.0;
  node->box.h = 1.0;
  return node;
}

gui_node_t gui_create_text(int col, int row)
{
  gui_node_t node = gui_node_create(GUI_LABEL);
  node->text.size = 1.0;
  node->text.col = col;
  node->text.row = row;
  node->text.text = calloc(col * row, sizeof(char));
  return node;
}

void gui_box_resize(gui_node_t node, float w, float h)
{
  node->box.w = w;
  node->box.h = h;
}

void gui_text_resize(gui_node_t node, float size)
{
  node->text.size = size;
}

void gui_text_printf(gui_node_t node, const char *format)
{
  snprintf(node->text.text, node->text.row * node->text.col, format);
}

void gui_node_move(gui_node_t node, float x, float y)
{
  node->x = x;
  node->y = y;
}

void gui_node_attach(gui_node_t node, gui_node_t child)
{
  if (!node->child) {
    node->child = child;
  } else {
    node->child->next = child;
  }
}

int gui_push_node(int rect_num, const gui_node_t node, space_t space)
{
  if (!node) {
    return rect_num;
  }
  
  rect_num = gui_push_node(rect_num, node->next, space);
  
  switch (node->type) {
  case GUI_BOX:
    rect_num = gui_push_box(rect_num, node, space);
    break;
  case GUI_LABEL:
    rect_num = gui_push_text(rect_num, node, space);
    break;
  }
  
  return rect_num;
}

int gui_push_box(int rect_num, gui_node_t node, space_t space)
{
  node->space = space;
  node->box.rect_ptr = rect_num++;
  gui_update_box(node);
  
  space_t new_space = (space_t) {
    .x = space.x + node->x * space.w,
    .y = space.y + node->y * space.h,
    .w = node->box.w * space.w,
    .h = node->box.h * space.h
  };
  
  return gui_push_node(rect_num, node->child, new_space);
}

int gui_push_text(int rect_num, gui_node_t node, space_t space)
{
  node->space = space;
  node->text.rect_ptr = rect_num;
  rect_num += node->text.row * node->text.col;
  gui_update_text(node);
  
  space_t new_space = (space_t) {
    .x = space.x + node->x * space.x,
    .y = space.y + node->y * space.y,
    .w = node->text.size * node->text.col * space.h,
    .h = node->text.size * node->text.row * space.h
  };
  
  return gui_push_node(rect_num, node->child, new_space);
}

void gui_update_node(const gui_node_t node)
{
  if (!node) {
    return;
  }
  
  gui_update_node(node->next);
  
  switch (node->type) {
  case GUI_BOX:
    gui_update_box(node);
    break;
  case GUI_LABEL:
    gui_update_text(node);
    break;
  }
}

void gui_update_box(const gui_node_t node)
{
  rect_t rect = {
    .x = node->space.x + node->x * node->space.w,
    .y = node->space.y + node->y * node->space.h,
    .w = node->box.w * node->space.w,
    .h = node->box.h * node->space.h,
    .tx = 0.0, .ty = 0.0, .tw = 1.0, .th = 1.0,
    .color = node->color
  };
  
  rect_update(&rect, node->box.rect_ptr);
}

void gui_update_text(const gui_node_t node)
{
  int col = 0;
  int row = 0;
  
  rect_t glyph = (rect_t) {
    .x = node->space.x + node->x * node->space.w,
    .y = node->space.y + node->y * node->space.h,
    .w = 5.0 / 8.0 * node->text.size * node->space.h,
    .h = node->text.size * node->space.h,
    .tw = 5.0 / 8.0, .th = 1.0,
    .color = node->color
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

void gui_destroy(gui_node_t node)
{
  if (!node) {
    return;
  }
  
  free(node->child);
  free(node->next);
  
  switch (node->type) {
  case GUI_BOX:
    break;
  case GUI_LABEL:
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
