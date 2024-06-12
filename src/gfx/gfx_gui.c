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
  float   x, y, w, h;
  float   tx, ty, tw, th;
  vector  color;
} ub_rect_t;

typedef struct {
  ub_rect_t rect[RECT_MAX];
} ub_rectdata_t;

typedef struct {
  int offset;
  int count;
} rectdef_t;

typedef enum {
  GUI_BOX,
  GUI_LABEL
} gui_type_t;

struct gui_node_s {
  union {
    struct {
      float w, h;
      rectdef_t def;
    } box;
    struct {
      float size;
      int max_text;
      char *text;
      rectdef_t def;
    } label;
  };
  float x, y;
  vector color;
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

static void gui_push_R(ub_rect_t *rectdata, int *num_rect, const gui_node_t node, float x, float y, float w, float h);
static void gui_push_box(ub_rect_t *rectdata, int *num_rect, const gui_node_t node, float x, float y, float w, float h);
static void gui_push_label(ub_rect_t *rectdata, int *num_rect, const gui_node_t node, float x, float y, float w, float h);

void gui_init(mesh_t mesh)
{
  char define_max[256];
  snprintf(define_max, sizeof(define_max), "#define RECT_MAX %i", RECT_MAX);
  
  shaderdata_t sd = shaderdata_create();
    shaderdata_line(sd, define_max, SD_VERT);
    shaderdata_source(sd, "assets/shader/gui/gui.vert", SD_VERT);
    shaderdata_source(sd, "assets/shader/gui/gui.frag", SD_FRAG);
    gui.shader = shader_load(sd);
    glUniformBlockBinding(gui.shader, glGetUniformBlockIndex(gui.shader, "rectdata"), 2);
  shaderdata_destroy(sd);
  
  gui.sheet = texture_load_image("assets/sheet/gui.png");
  gui.mesh = mesh;
  gui.top = 0;
  
  glGenBuffers(1, &gui.ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, gui.ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(ub_rectdata_t), NULL, GL_DYNAMIC_DRAW);
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
  static ub_rect_t rectdata[RECT_MAX];
  int num_rect = 0;
  
  gui_push_R(rectdata, &num_rect, node, 0.0, 0.0, 1.0, 1.0);
  
  gui_frame_t frame = gui.top;
  gui.top += num_rect;
  
  glBindBuffer(GL_UNIFORM_BUFFER, gui.ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, frame * sizeof(ub_rect_t), gui.top * sizeof(ub_rect_t), rectdata);
  
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

gui_node_t gui_create_label(int max_text)
{
  gui_node_t node = gui_node_create(GUI_LABEL);
  node->label.size = 1.0;
  node->label.text = calloc(max_text, sizeof(char));
  strcpy(node->label.text, "let x = 3 - 4; if \"TRUE\" =[]");
  return node;
}

void gui_box_resize(gui_node_t node, float w, float h)
{
  node->box.w = w;
  node->box.h = h;
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

void gui_push_R(ub_rect_t *rectdata, int *num_rect, const gui_node_t node, float x, float y, float w, float h)
{
  if (!node) {
    return;
  }
  
  gui_push_R(rectdata, num_rect, node->next, x, y, w, h);
  
  switch (node->type) {
  case GUI_BOX:
    gui_push_box(rectdata, num_rect, node, x, y, w, h);
    break;
  case GUI_LABEL:
    gui_push_label(rectdata, num_rect, node, x, y, w, h);
    break;
  }
}

void gui_push_box(ub_rect_t *rectdata, int *num_rect, const gui_node_t node, float x, float y, float w, float h)
{
  ub_rect_t rect = {
    .x = x + node->x * w * 960.0 / 1280.0,
    .y = y + node->y * h,
    .w = node->box.w * w,
    .h = node->box.h * h,
    .tx = 0.0, .ty = 0.0, .tw = 1.0, .th = 1.0,
    .color = node->color
  };
  rectdata[(*num_rect)++] = rect;
  gui_push_R(rectdata, num_rect, node->child, rect.x, rect.y, rect.w, rect.h);
}

void gui_push_label(ub_rect_t *rectdata, int *num_rect, const gui_node_t node, float x, float y, float w, float h)
{
  ub_rect_t glyph = {
    .x = x + node->x * w * 960.0 / 1280.0,
    .y = y + node->y * h,
    .w = 5.0 / 8.0 * node->label.size * h,
    .h = node->label.size * h
  };
  
  for (int i = 0; i < strlen(node->label.text); i++) {
    float u = 0.0;
    float v = 15.0;
    int letter = node->label.text[i];
    
    if (letter >= ' ' && letter <= '~') {
      letter -= ' ';
      u = (float) (letter % 16);
      v = 15.0 - floor(letter / 16.0);
    }
    
    rectdata[(*num_rect)++] = (ub_rect_t) {
      .x = glyph.x + i * glyph.w,
      .y = glyph.y,
      .w = glyph.w,
      .h = glyph.h,
      .tx = u, .ty = v, .tw = 5.0 / 8.0, .th = 1.0,
      .color = node->color
    };
  }
  
  for (int i = strlen(node->label.text); i < node->label.max_text; i++) {
    rectdata[(*num_rect)++] = (ub_rect_t) {
      .w = 0.0, .h = 0.0
    };
  }
  
  gui_push_R(rectdata, num_rect, node->child, glyph.x, glyph.y, glyph.w * strlen(node->label.text), glyph.h);
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
    free(node->label.text);
    break;
  }
  
  free(node);
}
