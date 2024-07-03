#include <gfx/gfx.h>
#include <gfx/texture.h>
#include <gfx/shader.h>
#include <gfx/mesh.h>
#include <gfx/camera.h>
#include <gfx/target.h>
#include <gfx/gfx_sprite.h>
#include <gfx/gfx_map.h>
#include <gfx/gui.h>
#include <stdio.h>
#include <GL/glew.h>

#define FOV (1.0 / 12.0)

#define SCR_WIDTH 1280
#define SCR_HEIGHT 960
#define SCR_SCALE 2
#define ASPECT_RATIO ((float) SCR_WIDTH / (float) SCR_HEIGHT)

#define VIEW_WIDTH (SCR_WIDTH / SCR_SCALE)
#define VIEW_HEIGHT (SCR_HEIGHT / SCR_SCALE)

#define MAX_VERTICES 1024 * 1024

struct {
  mesh_t quad;
  
  shader_t shroud;
  shader_t dither;
  shader_t blur;
  
  texture_t sheet;
  texture_t emit;
  
  texture_t depth;
  texture_t bloom;
  texture_t buffer[2];
  
  target_t to_buffer[2];
  target_t to_buffer_and_bloom;
  target_t to_bloom;
  
  GLuint ul_horizontal;
} gfx;

static shader_t frame_shader_load(const char *path);
static void gfx_pipeline_init();

void gfx_init()
{
  camera_init(FOV, FOV * ASPECT_RATIO);
  vbuffer_init(MAX_VERTICES);
  vbuffer_bind();
  gfx_pipeline_init();
  gui_init(gfx.quad);
  gfx_sprite_init(gfx.quad);
  gfx_map_init();
  
  glCullFace(GL_BACK);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

float t = 0.0;

void gfx_render(const game_t *gs)
{
  t += 0.015;
  
  camera_move(gs->view_pos, gs->view_rot);
  
  glViewport(0, 0, VIEW_WIDTH, VIEW_HEIGHT);
  
  target_bind(gfx.to_buffer_and_bloom);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    texture_bind(gfx.sheet, GL_TEXTURE_2D, 0);
    texture_bind(gfx.emit, GL_TEXTURE_2D, 1);
    
    camera_isometric();
    camera_update(identity());
    gfx_map_draw();
    
    camera_orthographic();
    camera_update(identity());
    gfx_sprite_draw(gs);
  target_unbind();
  
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);
  
  shader_bind(gfx.blur);
  for (int i = 0; i < 4; i++) {
    glUniform1i(gfx.ul_horizontal, i % 2);
    
    target_bind(gfx.to_buffer[1]);
      texture_bind(gfx.bloom, GL_TEXTURE_2D, 0);
      vbuffer_draw(gfx.quad);
    target_unbind();
    
    target_bind(gfx.to_bloom);
      texture_bind(gfx.buffer[1], GL_TEXTURE_2D, 0);
      vbuffer_draw(gfx.quad);
    target_unbind();
  }
  
  target_bind(gfx.to_buffer[1]);
    texture_bind(gfx.buffer[0], GL_TEXTURE_2D, 0);
    texture_bind(gfx.bloom, GL_TEXTURE_2D, 1);
    shader_bind(gfx.shroud);
    vbuffer_draw(gfx.quad);
  target_unbind();
  
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  
  glEnable(GL_BLEND);
  texture_bind(gfx.buffer[1], GL_TEXTURE_2D, 0);
  shader_bind(gfx.dither);
  vbuffer_draw(gfx.quad);
  gui_draw();
  glDisable(GL_BLEND);
  
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
}

void gfx_load_map(map_t map)
{
  gfx_map_load(map);
}

void gfx_pipeline_init()
{
  gfx.sheet = texture_load_image("assets/sheet/color.png");
  gfx.emit = texture_load_image("assets/sheet/emit.png");
  
  gfx.depth = texture_create(VIEW_WIDTH, VIEW_HEIGHT, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
  gfx.bloom = texture_create(VIEW_WIDTH, VIEW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE);
  gfx.to_bloom = target_create(2, GL_COLOR_ATTACHMENT0, gfx.bloom, GL_DEPTH_ATTACHMENT, gfx.depth);
  
  for (int i = 0; i < 2; i++) {
    gfx.buffer[i] = texture_create(VIEW_WIDTH, VIEW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE);
    gfx.to_buffer[i] = target_create(
      2,
      GL_COLOR_ATTACHMENT0, gfx.buffer[i],
      GL_DEPTH_ATTACHMENT, gfx.depth
    );
  }
  
  gfx.to_buffer_and_bloom = target_create(
    3,
    GL_COLOR_ATTACHMENT0, gfx.buffer[0],
    GL_COLOR_ATTACHMENT1, gfx.bloom,
    GL_DEPTH_ATTACHMENT, gfx.depth
  );
    
  gfx.shroud = frame_shader_load("assets/shader/fragment/shroud.frag");
  gfx.dither = frame_shader_load("assets/shader/fragment/dither.frag");
  gfx.blur = frame_shader_load("assets/shader/fragment/blur.frag");
  
  gfx.ul_horizontal = glGetUniformLocation(gfx.blur, "horizontal");
  
  shader_bind(gfx.shroud);
  glUniform1i(glGetUniformLocation(gfx.shroud, "bloom"), 1);
  
  meshdata_t md = meshdata_create();
    meshdata_add_quad(md, identity(), identity());
    gfx.quad = vbuffer_add(md);
  meshdata_destroy(md);
}

void gfx_deinit()
{
  gfx_map_deinit();
  gfx_sprite_deinit();
  gui_deinit();
  target_destroy(gfx.to_bloom);
  target_destroy(gfx.to_buffer[0]);
  target_destroy(gfx.to_buffer[1]);
  texture_destroy(gfx.bloom);
  texture_destroy(gfx.buffer[0]);
  texture_destroy(gfx.buffer[1]);
  texture_destroy(gfx.depth);
  texture_destroy(gfx.sheet);
  shader_destroy(gfx.shroud);
  shader_destroy(gfx.dither);
  shader_destroy(gfx.blur);
  vbuffer_deinit();
  camera_deinit();
}

shader_t frame_shader_load(const char *path)
{
  shaderdata_t sd = shaderdata_create();
    shaderdata_source(sd, "assets/shader/vertex/frame.vert", SD_VERT);
    shaderdata_source(sd, path, SD_FRAG);
    shader_t shader = shader_load(sd);
  shaderdata_destroy(sd);
  return shader;
}
