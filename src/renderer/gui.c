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
  texture_t sheet;
  shader_t  shader;
  GLuint    ubo;
  int       num_rect;
} gui;

static void rect_solid(float x, float y, float w, float h, vector color);
static void rect_glyph(float x, float y, float size, int letter, int pos, vector color);

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
  
  char sentence[] = ">god";
  
  for (int i = 0; i < sizeof(sentence); i++) {
    rect_glyph(0.0, 0.0, 0.015625 * 2.0, sentence[i], i, vec4(1.0, 1.0, 1.0, 1.0));
  }
}

void rect_glyph(float x, float y, float size, int letter, int pos, vector color)
{
  float u = 0.0;
  float v = 15.0;
  
  if (letter >= ' ' && letter <= '~') {
    letter -= ' ';
    u = (float) (letter % 16);
    v = 15.0 - floor(letter / 16.0);
  }
  
  ub_rectdata_t rectdata = {
    .rect = {
      {
        .p_pos = { x + pos * 5.0 / 8.0 * size, y },
        .p_size = { 5.0 / 8.0 * size, size },
        .uv_pos = { u, v },
        .uv_size = { 5.0 / 8.0, 1.0 },
        .color = { color.x, color.y, color.z, color.w }
      }
    }
  };
  
  glBindBuffer(GL_UNIFORM_BUFFER, gui.ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(ub_rect_t) * gui.num_rect++, sizeof(ub_rect_t), rectdata.rect);
}

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

void gui_draw(game_t *gs)
{
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
