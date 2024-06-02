#include <renderer/r_sprite.h>
#include <renderer/mesh.h>
#include <renderer/shader.h>
#include <renderer/texture.h>
#include <lib/math3d.h>
#include <GL/glew.h>
#include <stdio.h>

#define RECT_MAX 256

typedef struct {
  float p_pos[2];
  float p_size[2];
  float uv_pos[2];
  float uv_size[2];
  float color[4];
} ub_rect_t;

typedef struct {
  ub_rect_t rect[RECT_MAX];
} ub_rectdata_t;

struct {
  mesh_t    mesh;
  shader_t  shader;
  GLuint    ubo;
  int       num_rect;
} gui;

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
  
  gui.mesh = mesh;
  
  glGenBuffers(1, &gui.ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, gui.ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(ub_rectdata_t), NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 2, gui.ubo);
}

void gui_draw(game_t *gs)
{
  health_t *h = entity_get_component(gs, gs->boss, health);
  
  ub_rectdata_t rectdata = {
    .rect = {
      {
        .p_pos = { 0.0, 0.0 },
        .p_size = { h->hp / (float) h->max_hp, 0.02 },
        .uv_pos = { 0.0, 0.0 },
        .uv_size = { 1.0, 1.0 },
        .color = { 1.0, 0.0, 0.0, 0.5 }
      }
    }
  };
  
  glBindBuffer(GL_UNIFORM_BUFFER, gui.ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ub_rect_t), rectdata.rect);
  
  shader_bind(gui.shader);
  glDrawArraysInstanced(GL_TRIANGLES, gui.mesh.offset, gui.mesh.count, 1);
}

void gui_deinit()
{
  shader_destroy(gui.shader);
  glDeleteBuffers(1, &gui.ubo);
}
