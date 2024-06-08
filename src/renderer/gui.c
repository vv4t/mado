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
#define TEXT_MAX 64

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
} rectbuf_t;

typedef struct {
  rectbuf_t rb;
  float     x, y, w, h;
  vector    color;
} GUI_box_t;

typedef struct {
  rectbuf_t rb;
  float     x, y, size;
  vector    color;
  char      text[TEXT_MAX];
} GUI_label_t;

typedef struct {
  GUI_box_t   bar;
  GUI_label_t label;
} GUI_health_t;

struct {
  mesh_t    mesh;
  texture_t sheet;
  shader_t  shader;
  GLuint    ubo;
  int       num_rect;
  
  GUI_health_t  health[4];
} GUI;

static GUI_health_t GUI_health_create(int num, const char *name);

static GUI_box_t    GUI_box_create(float x, float y, float w, float h, vector color);
static void         GUI_box_update(const GUI_box_t *box);

static GUI_label_t  GUI_label_create(float x, float y, float size, int max_len, const char *text, vector color);
static void         GUI_label_update(const GUI_label_t *label);

static rectbuf_t    rect_define(int count);
static void         rect_update(const rectbuf_t *rb, const ub_rect_t *data);

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
  GUI.num_rect = 0;
  
  glGenBuffers(1, &GUI.ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, GUI.ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(ub_rectdata_t), NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 2, GUI.ubo);
  
  GUI.health[0] = GUI_health_create(0, "MR.SCYTHER");
  GUI.health[1] = GUI_health_create(1, "MR.MAGE");
  GUI.health[2] = GUI_health_create(2, "MR.WARRIOR");
}

float get_health_width(const health_t *health)
{
  return health->hp / (float) health->max_hp * 1280.0 / 960.0 - 0.015 * 2;
}

void GUI_draw(const game_t *gs)
{
  for (int i = 0; i < 3; i++) {
    GUI.health[i].bar.w = get_health_width(&gs->health[gs->boss[i]]);
    GUI_box_update(&GUI.health[i].bar);
  }
  
  shader_bind(GUI.shader);
  texture_bind(GUI.sheet, GL_TEXTURE_2D, 0);
  glDrawArraysInstanced(GL_TRIANGLES, GUI.mesh.offset, GUI.mesh.count, GUI.num_rect);
}

void GUI_deinit()
{
  texture_destroy(GUI.sheet);
  shader_destroy(GUI.shader);
  glDeleteBuffers(1, &GUI.ubo);
}

GUI_health_t GUI_health_create(int num, const char *name)
{
  float pt = 0.025;
  float margin = 0.01;
  float pad = 0.005;
  
  int n = strlen(name);
  
  return (GUI_health_t) {
    .bar = GUI_box_create(
      margin, 1.0 - (1 + num) * (margin + pt + pad * 2),
      1280.0 / 960.0 - (pad + margin) * 2, pt + pad * 2,
      vec4(1.0, 0.0, 0.0, 0.8)
    ),
    .label = GUI_label_create(
      0.5 - n * pt * 5.0 / 8.0 * 0.5,
      1.0 - (1 + num) * (margin + pad + pt) - num * pad,
      pt, n, name,
      vec4(1.0, 1.0, 1.0, 1.0)
    )
  };
}

GUI_box_t GUI_box_create(float x, float y, float w, float h, vector color)
{
  GUI_box_t box = (GUI_box_t) {
    .rb = rect_define(1),
    .x = x, .y = y, .w = w, .h = h,
    .color = color
  };
  
  GUI_box_update(&box);
  
  return box;
}

void GUI_box_update(const GUI_box_t *box)
{
  ub_rect_t rect = {
    .x = box->x, .y = box->y, .w = box->w, .h = box->h,
    .tx = 0.0, .ty = 0.0, .tw = 1.0, .th = 1.0,
    .color = box->color
  };
  
  rect_update(&box->rb, &rect);
}

GUI_label_t GUI_label_create(float x, float y, float size, int max_len, const char *text, vector color)
{
  GUI_label_t label = (GUI_label_t) {
    .rb = rect_define(max_len),
    .x = x, .y = y, .size = size,
    .color = color
  };
  
  strncpy(label.text, text, sizeof(label.text));
  
  GUI_label_update(&label);
  
  return label;
}

void GUI_label_update(const GUI_label_t *label)
{
  static ub_rect_t rect[256];
  
  for (int i = 0; i < strlen(label->text); i++) {
    float u = 0.0;
    float v = 15.0;
    int letter = label->text[i];
    
    if (letter >= ' ' && letter <= '~') {
      letter -= ' ';
      u = (float) (letter % 16);
      v = 15.0 - floor(letter / 16.0);
    }
    
    rect[i] = (ub_rect_t) {
      .x = label->x + i * 5.0 / 8.0 * label->size, .y = label->y,
      .w = 5.0 / 8.0 * label->size, .h = label->size,
      .tx = u, .ty = v, .tw = 5.0 / 8.0, .th = 1.0,
      .color = label->color
    };
  }
  
  for (int i = strlen(label->text); i < label->rb.count; i++) {
    rect[i] = (ub_rect_t) {
      .w = 0.0, .h = 0.0
    };
  }
  
  rect_update(&label->rb, rect);
}

rectbuf_t rect_define(int count)
{
  if (GUI.num_rect + count >= RECT_MAX) {
    LOG_ERROR("out of memory");
  }
  
  int offset = GUI.num_rect;
  GUI.num_rect += count;
  
  return (rectbuf_t) {
    .offset = offset,
    .count = count
  };
}

void rect_update(const rectbuf_t *rb, const ub_rect_t *data)
{
  int offset = sizeof(ub_rect_t) * rb->offset;
  int size = sizeof(ub_rect_t) * rb->count;
  
  glBindBuffer(GL_UNIFORM_BUFFER, GUI.ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}
