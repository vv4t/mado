#include <gfx/sprite_renderer.h>
#include <gfx/r_def.h>
#include <gfx/camera.h>
#include <gfx/mesh.h>
#include <gfx/shader.h>
#include <gfx/texture.h>
#include <lib/math3d.h>
#include <GL/glew.h>
#include <stdio.h>

#define SPRITE_MAX 512
#define OCCLUDE_RADIUS 32.0

typedef struct {
  matrix T_p;
  matrix T_uv;
} ub_sprite_t;

typedef struct {
  ub_sprite_t sprite[SPRITE_MAX];
} ub_spritedata_t;

struct {
  mesh_t    mesh;
  shader_t  shader;
  GLuint    ubo;
} sprite_renderer;

void sprite_renderer_init(mesh_t mesh)
{
  char define_max[256];
  snprintf(define_max, sizeof(define_max), "#define SPRITE_MAX %i", SPRITE_MAX);
  
  shaderdata_t sd = shaderdata_create();
    camera_shader_import(sd);
    shaderdata_line(sd, define_max, SD_VERT);
    shaderdata_source(sd, "assets/shader/vertex/sprite.vert", SD_VERT);
    shaderdata_source(sd, "assets/shader/fragment/world.frag", SD_FRAG);
    sprite_renderer.shader = shader_load(sd);
    camera_shader_attach(sprite_renderer.shader);
    glUniformBlockBinding(sprite_renderer.shader, glGetUniformBlockIndex(sprite_renderer.shader, "spritedata"), 1);
    glUniform1i(glGetUniformLocation(sprite_renderer.shader, "emit"), 1);
  shaderdata_destroy(sd);
  
  sprite_renderer.mesh = mesh;
  
  glGenBuffers(1, &sprite_renderer.ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, sprite_renderer.ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(ub_spritedata_t), NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 1, sprite_renderer.ubo);
}

void sprite_renderer_draw(const game_t *gs)
{
  static ub_spritedata_t spritedata;
  
  shader_bind(sprite_renderer.shader);
  
  matrix inv_view = inverse(mat3_from_mat4(camera_get_view()));
  
  int num_sprite = 0;
  for (entity_t e = 0; e < gs->num_entities; e++) {
    if (!entity_match(gs, e, C_transform | C_sprite)) {
      continue;
    }
    
    const transform_t *t = entity_get_component(gs, e, transform);
    const sprite_t *s = entity_get_component(gs, e, sprite);
    
    float view_dist = length_squared(mdotv(camera_get_view(), t->position));
    
    if (view_dist >= OCCLUDE_RADIUS * OCCLUDE_RADIUS) {
      continue;
    }
    
    matrix T_p = mat3(
      vec3( 1,  0,  0),
      vec3( 0,  1, -1),
      vec3( 0,  0,  1)
    );
    
    if (!s->orient) {
      T_p = mdotm(mat3_from_mat4(camera_get_view()), T_p);
    }
    
    T_p = mdotm(mdotm(translate(vec3(0, 0, -1)), fscale(0.5)), T_p);
    T_p = mdotm(rotate_z(s->rotation), T_p);
    T_p = mdotm(T_p, translate(vec2(0.0, 0.5 - t->position.z)));
    T_p = mdotm(T_p, scale(t->scale));
    T_p = mdotm(T_p, inv_view);
    T_p = mdotm(T_p, translate(t->position));
    
    matrix T_uv = identity();
    T_uv = mdotm(T_uv, scale(vec3(s->tw, s->th, 1)));
    T_uv = mdotm(T_uv, translate(vec2(s->tx, s->ty)));
    T_uv = mdotm(T_uv, fscale(1.0 / SHEET_SIZE));
    
    spritedata.sprite[num_sprite++] = (ub_sprite_t) {
      .T_p = T_p,
      .T_uv = T_uv
    };
    
    if (num_sprite >= SPRITE_MAX) {
      glBindBuffer(GL_UNIFORM_BUFFER, sprite_renderer.ubo);
      glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ub_sprite_t) * num_sprite, spritedata.sprite);
      glDrawArraysInstanced(GL_TRIANGLES, sprite_renderer.mesh.offset, sprite_renderer.mesh.count, num_sprite);
      num_sprite = 0;
    }
  }
  
  glBindBuffer(GL_UNIFORM_BUFFER, sprite_renderer.ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ub_sprite_t) * num_sprite, spritedata.sprite);
  glDrawArraysInstanced(GL_TRIANGLES, sprite_renderer.mesh.offset, sprite_renderer.mesh.count, num_sprite);
}

void sprite_renderer_deinit()
{
  shader_destroy(sprite_renderer.shader);
  glDeleteBuffers(1, &sprite_renderer.ubo);
}
