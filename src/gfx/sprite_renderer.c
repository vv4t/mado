#include <gfx/sprite_renderer.h>
#include <gfx/r_def.h>
#include <gfx/camera.h>
#include <gfx/mesh.h>
#include <gfx/shader.h>
#include <gfx/texture.h>
#include <lib/math3d.h>
#include <GL/glew.h>
#include <stdio.h>
#include <string.h>

#define PROP_MAX 64
#define SPRITE_MAX 512
#define OCCLUDE_RADIUS 32.0

typedef struct {
  matrix T_p;
  matrix T_uv;
} ub_sprite_t;

typedef struct {
  ub_sprite_t sprite[SPRITE_MAX];
  int num_sprites;
} spritedata_t;

typedef struct {
  transform_t t;
  sprite_t s;
} prop_t;

struct {
  mesh_t    mesh;
  shader_t  shader;
  GLuint    ubo;
  prop_t    props[PROP_MAX];
  int       num_props;
} sprite_renderer;

static const animation_t mr_fire_attack = { .tx = 8, .ty = 0, .tw = 1, .th = 1, .framecount = 3, .frametime = 0.125 };

static void spritedata_add(spritedata_t *spritedata, const transform_t *t, const sprite_t *s, matrix inv_view);
static void spritedata_flush(spritedata_t *spritedata);

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
  sprite_renderer.num_props = 0;
  
  glGenBuffers(1, &sprite_renderer.ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, sprite_renderer.ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(spritedata_t), NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 1, sprite_renderer.ubo);
}

void sprite_renderer_load_props(map_t map)
{
  static const struct {
    const char *match;
    float scale;
    animation_t animation;
  } prop_table[] = {
    { .match = "candle", .scale = 0.75, .animation = { .tx = 0, .ty = 4, .tw = 1, .th = 1, } }
  };
  
  int num_prop_table = sizeof(prop_table) / sizeof(prop_table[0]);
  
  sprite_renderer.num_props = 0;
  landmark_t lm = map_get_group(map, "props");
  
  while (lm) {
    prop_t *prop = &sprite_renderer.props[sprite_renderer.num_props++];
    prop->t = create_transform();
    prop->s = create_sprite();
    prop->t.position = landmark_get_position(lm);
    
    for (int i = 0; i < num_prop_table; i++) {
      const char *match = prop_table[i].match;
      if (strncmp(match, landmark_get_name(lm), strlen(match)) == 0) {
        prop->t.scale = fdotv(prop_table[i].scale, vec3(1.0, 1.0, 1.0));
        sprite_play(&prop->s, &prop_table[i].animation);
      }
    }
    
    lm = landmark_next(lm);
  }
}

void sprite_renderer_draw(const game_t *gs)
{
  static spritedata_t spritedata;
  
  shader_bind(sprite_renderer.shader);
  matrix inv_view = rotate_z(gs->view_rot);
  
  for (int i = 0; i < sprite_renderer.num_props; i++) {
    const transform_t *t = &sprite_renderer.props[i].t;
    const sprite_t *s = &sprite_renderer.props[i].s;
    
    spritedata_add(&spritedata, t, s, inv_view);
    
    if (spritedata.num_sprites >= SPRITE_MAX) {
      spritedata_flush(&spritedata);
    }
  }
  
  for (entity_t e = 0; e < gs->num_entities; e++) {
    if (!entity_match(gs, e, C_transform | C_sprite)) {
      continue;
    }
    
    const transform_t *t = entity_get_component(gs, e, transform);
    const sprite_t *s = entity_get_component(gs, e, sprite);
    
    spritedata_add(&spritedata, t, s, inv_view);
    
    if (spritedata.num_sprites >= SPRITE_MAX) {
      spritedata_flush(&spritedata);
    }
  }
  
  spritedata_flush(&spritedata);
}

void spritedata_flush(spritedata_t *spritedata)
{
  glBindBuffer(GL_UNIFORM_BUFFER, sprite_renderer.ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ub_sprite_t) * spritedata->num_sprites, spritedata->sprite);
  glDrawArraysInstanced(GL_TRIANGLES, sprite_renderer.mesh.offset, sprite_renderer.mesh.count, spritedata->num_sprites);
  spritedata->num_sprites = 0;
}

void spritedata_add(spritedata_t *spritedata, const transform_t *t, const sprite_t *s, matrix inv_view)
{
  float view_dist = length_squared(mdotv(camera_get_view(), t->position));
  
  if (view_dist >= OCCLUDE_RADIUS * OCCLUDE_RADIUS) {
    return;
  }
  
  matrix T_p = mat3(
    vec3( 1,  0,  0 ),
    vec3( 0,  1, -1 ),
    vec3( 0,  0,  1 )
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
  
  spritedata->sprite[spritedata->num_sprites++] = (ub_sprite_t) {
    .T_p = T_p,
    .T_uv = T_uv
  };
}

void sprite_renderer_deinit()
{
  shader_destroy(sprite_renderer.shader);
  glDeleteBuffers(1, &sprite_renderer.ubo);
}
