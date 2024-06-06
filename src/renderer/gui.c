#include <renderer/r_sprite.h>
#include <renderer/mesh.h>
#include <renderer/shader.h>
#include <renderer/texture.h>
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

typedef int GUI_frame_t;

typedef enum {
  GUI_BOX,
  GUI_LABEL
} GUI_type_t;

typedef struct GUI_node_s {
  union {
    struct {
      float w, h;
    } box;
    struct {
      float size;
      int max_text;
      char *text;
    } label;
  };
  float x, y;
  vector color;
  GUI_type_t type;
  struct GUI_node_s *next;
  struct GUI_node_s *child;
  rectdef_t def;
} GUI_node_t;

struct {
  mesh_t      mesh;
  texture_t   sheet;
  shader_t    shader;
  GLuint      ubo;
  GUI_frame_t top;
} GUI;

static GUI_node_t   *GUI_create_label(float x, float y, float size, const char *text, int max_text, vector color);
static GUI_node_t   *GUI_create_box(float x, float y, float w, float h, vector color);
static void         GUI_add_child(GUI_node_t *node, GUI_node_t *child);
static void         GUI_destroy(GUI_node_t *node);

static GUI_frame_t  GUI_push(const GUI_node_t *node);
static void         GUI_push_R(ub_rect_t *rectdata, int *num_rect, const GUI_node_t *node, float x, float y, float w, float h);
static void         GUI_push_box(ub_rect_t *rectdata, int *num_rect, const GUI_node_t *node, float x, float y, float w, float h);
static void         GUI_push_label(ub_rect_t *rectdata, int *num_rect, const GUI_node_t *node, float x, float y, float w, float h);
static void         GUI_reset(GUI_frame_t frame);

void GUI_init(mesh_t mesh)
{
  char define_max[256];
  snprintf(define_max, sizeof(define_max), "#define RECT_MAX %i", RECT_MAX);
  
  shaderdata_t sd = shaderdata_create();
    shaderdata_line(sd, define_max, SD_VERT);
    shaderdata_source(sd, "assets/shader/vertex/GUI.vert", SD_VERT);
    shaderdata_source(sd, "assets/shader/fragment/GUI.frag", SD_FRAG);
    GUI.shader = shader_load(sd);
    glUniformBlockBinding(GUI.shader, glGetUniformBlockIndex(GUI.shader, "rectdata"), 2);
  shaderdata_destroy(sd);
  
  GUI.sheet = texture_load_image("assets/sheet/GUI.png");
  GUI.mesh = mesh;
  GUI.top = 0;
  
  glGenBuffers(1, &GUI.ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, GUI.ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(ub_rectdata_t), NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 2, GUI.ubo);
  
  GUI_node_t *rect1 = GUI_create_box(0.01, 0.01, 1280.0 / 960.0 - 0.02, 0.05, vec4(1.0, 0.0, 0.0, 1.0));
  GUI_node_t *label1 = GUI_create_label(0.1 * 0.05, 0.1, 1.0, "hi there", 32, vec4(1.0, 1.0, 1.0, 1.0));
  GUI_add_child(rect1, label1);
  
  GUI_push(rect1);
}

void GUI_draw(const game_t *gs)
{
  shader_bind(GUI.shader);
  texture_bind(GUI.sheet, GL_TEXTURE_2D, 0);
  glDrawArraysInstanced(GL_TRIANGLES, GUI.mesh.offset, GUI.mesh.count, GUI.top);
}

void GUI_deinit()
{
  texture_destroy(GUI.sheet);
  shader_destroy(GUI.shader);
  glDeleteBuffers(1, &GUI.ubo);
}

GUI_frame_t GUI_push(const GUI_node_t *node)
{
  static ub_rect_t rectdata[RECT_MAX];
  int num_rect = 0;
  
  GUI_push_R(rectdata, &num_rect, node, 0.0, 0.0, 1.0, 1.0);
  
  GUI_frame_t frame = GUI.top;
  GUI.top += num_rect;
  
  glBindBuffer(GL_UNIFORM_BUFFER, GUI.ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, frame * sizeof(ub_rect_t), GUI.top * sizeof(ub_rect_t), rectdata);
  
  return frame;
}

void GUI_reset(GUI_frame_t frame)
{
  GUI.top = frame;
}

GUI_node_t *GUI_create_box(float x, float y, float w, float h, vector color)
{
  GUI_node_t *node = malloc(sizeof(*node));
  node->x = x;
  node->y = y;
  node->color = color;
  node->type = GUI_BOX;
  node->next = NULL;
  node->child = NULL;
  node->box.w = w;
  node->box.h = h;
  return node;
}

GUI_node_t *GUI_create_label(float x, float y, float size, const char *text, int max_text, vector color)
{
  GUI_node_t *node = malloc(sizeof(*node));
  node->x = x;
  node->y = y;
  node->color = color;
  node->type = GUI_LABEL;
  node->next = NULL;
  node->child = NULL;
  node->label.size = size;
  node->label.text = calloc(max_text, sizeof(char));
  strncpy(node->label.text, text, strlen(text) + 1);
  return node;
}

void GUI_add_child(GUI_node_t *node, GUI_node_t *child)
{
  if (!node->child) {
    node->child = child;
  } else {
    node->child->next = child;
  }
}

void GUI_push_R(ub_rect_t *rectdata, int *num_rect, const GUI_node_t *node, float x, float y, float w, float h)
{
  if (!node) {
    return;
  }
  
  GUI_push_R(rectdata, num_rect, node->next, x, y, w, h);
  
  switch (node->type) {
  case GUI_BOX:
    GUI_push_box(rectdata, num_rect, node, x, y, w, h);
    break;
  case GUI_LABEL:
    GUI_push_label(rectdata, num_rect, node, x, y, w, h);
    break;
  }
}

void GUI_push_box(ub_rect_t *rectdata, int *num_rect, const GUI_node_t *node, float x, float y, float w, float h)
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
  GUI_push_R(rectdata, num_rect, node->child, rect.x, rect.y, rect.w, rect.h);
}

void GUI_push_label(ub_rect_t *rectdata, int *num_rect, const GUI_node_t *node, float x, float y, float w, float h)
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
  
  GUI_push_R(rectdata, num_rect, node->child, glyph.x, glyph.y, glyph.w * strlen(node->label.text), glyph.h);
}

void GUI_destroy(GUI_node_t *node)
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
