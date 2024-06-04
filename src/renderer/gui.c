#include <renderer/r_sprite.h>
#include <renderer/mesh.h>
#include <renderer/shader.h>
#include <renderer/texture.h>
#include <lib/math3d.h>
#include <lib/log.h>
#include <GL/glew.h>
#include <string.h>
#include <stdio.h>

#define RECT_MAX 256

typedef struct {
  float x, y, w, h;
  float tx, ty, tw, th;
  vector color;
} ub_rect_t;

typedef struct {
  ub_rect_t rect[RECT_MAX];
} ub_rectdata_t;

typedef struct {
  int offset;
  int count;
} rectdef_t;

typedef struct {
  rectdef_t rd;
  float x, y, w, h;
  vector color;
} mgui_box_t;

typedef struct {
  rectdef_t rd;
  float x, y, size;
  vector color;
  char text[128];
} mgui_label_t;

struct {
  mesh_t    mesh;
  texture_t sheet;
  shader_t  shader;
  GLuint    ubo;
  int       num_rect;
  
  mgui_box_t    health_bar;
  mgui_label_t  health_label;
} gui;

void mgui_box_update(const mgui_box_t *box);
mgui_box_t mgui_box_create(float x, float y, float w, float h, vector color);
mgui_label_t mgui_label_create(float x, float y, float size, int max_len, const char *text, vector color);
void mgui_label_update(const mgui_label_t *label);
rectdef_t rect_define(int count);
void rect_update(const rectdef_t *rd, const ub_rect_t *data);

void gui_init(mesh_t mesh)
{
  char define_max[256];
  snprintf(define_max, sizeof(define_max), "#define RECT_MAX %i", RECT_MAX);
  
  shaderdata_t sd = shaderdata_create();
    shaderdata_line(sd, define_max, SD_VERT);
    shaderdata_source(sd, "assets/shader/vertex/gui.vert", SD_VERT);
    shaderdata_source(sd, "assets/shader/fragment/gui.frag", SD_FRAG);
    gui.shader = shader_load(sd);
    glUniformBlockBinding(gui.shader, glGetUniformBlockIndex(gui.shader, "rectdata"), 2);
  shaderdata_destroy(sd);
  
  gui.sheet = texture_load_image("assets/sheet/gui.png");
  gui.mesh = mesh;
  gui.num_rect = 0;
  
  glGenBuffers(1, &gui.ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, gui.ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(ub_rectdata_t), NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 2, gui.ubo);
  
  gui.health_bar = mgui_box_create(0.01, 0.01, 0.99, 0.0625, vec4(1.0, 0.0, 0.0, 0.25));
  gui.health_label = mgui_label_create(0.02, 0.02, 0.05, 16, "HP100", vec4(1.0, 1.0, 1.0, 1.0));
}

void gui_draw(const game_t *gs)
{
  gui.health_bar.w = gs->health[gs->boss].hp / (float) gs->health[gs->boss].max_hp;
  mgui_box_update(&gui.health_bar);
  
  snprintf(gui.health_label.text, sizeof(gui.health_label.text), "HP%i", gs->health[gs->boss].hp);
  mgui_label_update(&gui.health_label);
  
  shader_bind(gui.shader);
  texture_bind(gui.sheet, GL_TEXTURE_2D, 0);
  glDrawArraysInstanced(GL_TRIANGLES, gui.mesh.offset, gui.mesh.count, gui.num_rect);
}

void gui_deinit()
{
  texture_destroy(gui.sheet);
  shader_destroy(gui.shader);
  glDeleteBuffers(1, &gui.ubo);
}

void mgui_box_update(const mgui_box_t *box)
{
  ub_rect_t rect = {
    .x = box->x, .y = box->y, .w = box->w, .h = box->h,
    .tx = 0.0, .ty = 0.0, .tw = 1.0, .th = 1.0,
    .color = box->color
  };
  
  rect_update(&box->rd, &rect);
}

mgui_box_t mgui_box_create(float x, float y, float w, float h, vector color)
{
  mgui_box_t box = (mgui_box_t) {
    .rd = rect_define(1),
    .x = x, .y = y, .w = w, .h = h,
    .color = color
  };
  
  mgui_box_update(&box);
  
  return box;
}

mgui_label_t mgui_label_create(float x, float y, float size, int max_len, const char *text, vector color)
{
  mgui_label_t label = (mgui_label_t) {
    .rd = rect_define(max_len),
    .x = x, .y = y, .size = size,
    .color = color
  };
  
  strncpy(label.text, text, sizeof(label.text));
  
  mgui_label_update(&label);
  
  return label;
}

void mgui_label_update(const mgui_label_t *label)
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
  
  for (int i = strlen(label->text); i < label->rd.count; i++) {
    rect[i] = (ub_rect_t) {
      .w = 0.0, .h = 0.0
    };
  }
  
  rect_update(&label->rd, rect);
}

rectdef_t rect_define(int count)
{
  if (gui.num_rect + count >= RECT_MAX) {
    LOG_ERROR("out of memory");
  }
  
  int offset = gui.num_rect;
  gui.num_rect += count;
  
  return (rectdef_t) {
    .offset = offset,
    .count = count
  };
}

void rect_update(const rectdef_t *rd, const ub_rect_t *data)
{
  int offset = sizeof(ub_rect_t) * rd->offset;
  int size = sizeof(ub_rect_t) * rd->count;
  
  glBindBuffer(GL_UNIFORM_BUFFER, gui.ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

/*

void rect_solid(float x, float y, float w, float h, vector color)
{
  ub_rectdata_t rectdata = {
    .rect = {
      {
        .p_pos = { x, y },
        .p_size = { w, h },
        .uv_pos = { 0.0, 0.0 },
        .uv_size = { 1.0, 1.0 },
        .color = { color.x, color.y, color.z, color.w }
      }
    }
  };
  
  glBindBuffer(GL_UNIFORM_BUFFER, gui.ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(ub_rect_t) * gui.num_rect++, sizeof(ub_rect_t), rectdata.rect);
}
*/
