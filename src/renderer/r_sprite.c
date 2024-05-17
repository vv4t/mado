#include <renderer/r_sprite.h>
#include <renderer/camera.h>
#include <renderer/mesh.h>
#include <renderer/shader.h>
#include <renderer/texture.h>
#include <math3d.h>
#include <GL/glew.h>
#include <stdio.h>

#define SPRITE_MAX 32

typedef struct {
  matrix T_p;
  matrix T_uv;
} ub_sprite_t;

typedef struct {
  ub_sprite_t sprite[SPRITE_MAX];
  int num_sprite;
} ub_spritedata_t;

struct {
  mesh_t    mesh;
  shader_t  shader;
  texture_t sheet;
  GLuint    ubo;
} r_sprite;

void r_sprite_init()
{
  matrix T_p = mat3(
    vec3( 1,  0,  0),
    vec3( 0,  1, -1),
    vec3( 0,  0,  1)
  );
  
  T_p = mdotm(mdotm(translate(vec3(0, 0, -1)), fscale(0.5)), T_p);
  
  meshdata_t md = meshdata_create();
    meshdata_add_quad(md, T_p, identity());
    r_sprite.mesh = vbuffer_add(md);
  meshdata_destroy(md);
  
  shaderdata_t sd = shaderdata_create();
    camera_shader_import(sd);
    shaderdata_source(sd, "assets/shader/sprite/shader.vert", SD_VERT);
    shaderdata_source(sd, "assets/shader/sprite/shader.frag", SD_FRAG);
    r_sprite.shader = shader_load(sd);
    camera_shader_attach(r_sprite.shader);
    glUniformBlockBinding(r_sprite.shader, glGetUniformBlockIndex(r_sprite.shader, "spritedata"), 1);
  shaderdata_destroy(sd);
  
  r_sprite.sheet = texture_load_image("assets/sheet/1.png");
  
  glGenBuffers(1, &r_sprite.ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, r_sprite.ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(ub_spritedata_t), NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 1, r_sprite.ubo);
}

void r_sprite_draw(const game_t *gs)
{
  static ub_spritedata_t spritedata;
  spritedata.num_sprite = 0;
  
  for (entity_t e = 0; e < gs->edict.num_entities; e++) {
    if (ENTITY_MATCH(gs->edict, e, C_transform | C_sprite)) {
      continue;
    }
    
    if (spritedata.num_sprite >= SPRITE_MAX) {
      continue;
    }
    
    const transform_t *t = ENTITY_GET_COMPONENT(gs->edict, e, transform);
    const sprite_t *s = ENTITY_GET_COMPONENT(gs->edict, e, sprite);
    
    matrix T_p = identity();
    T_p = mdotm(T_p, translate(vec2(0.0, 0.5)));
    T_p = mdotm(T_p, inverse(mat3_from_mat4(camera_get_view())));
    T_p = mdotm(T_p, translate(t->position));
    
    matrix T_uv = identity();
    T_uv = mdotm(T_uv, translate(vec2(s->tx, s->ty)));
    T_uv = mdotm(T_uv, fscale(1.0 / 8.0));
    
    spritedata.sprite[spritedata.num_sprite++] = (ub_sprite_t) {
      .T_p = T_p,
      .T_uv = T_uv
    };
  }
  
  glBindBuffer(GL_UNIFORM_BUFFER, r_sprite.ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(spritedata), &spritedata);
  
  shader_bind(r_sprite.shader);
  texture_bind(r_sprite.sheet, GL_TEXTURE_2D, 0);
  glDrawArraysInstanced(GL_TRIANGLES, r_sprite.mesh.offset, r_sprite.mesh.count, spritedata.num_sprite);
}

void r_sprite_deinit()
{
  shader_destroy(r_sprite.shader);
  texture_destroy(r_sprite.sheet);
}
