#include <renderer/renderer.h>
#include <renderer/texture.h>
#include <renderer/shader.h>
#include <renderer/mesh.h>
#include <renderer/camera.h>
#include <renderer/target.h>
#include <renderer/r_sprite.h>
#include <renderer/r_map.h>
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
} renderer;

static shader_t frame_shader_load(const char *path);
static void renderer_pipeline_init();

void renderer_init()
{
  camera_init(FOV, FOV * ASPECT_RATIO);
  vbuffer_init(MAX_VERTICES);
  vbuffer_bind();
  renderer_pipeline_init();
  r_sprite_init(renderer.quad);
  r_map_init();
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

float t = 0.0;

void renderer_render(const game_t *gs)
{
  t += 0.015;
  
  camera_move(gs->view_position, gs->view_rotation);
  
  glViewport(0, 0, VIEW_WIDTH, VIEW_HEIGHT);
  
  target_bind(renderer.to_buffer_and_bloom);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    texture_bind(renderer.sheet, GL_TEXTURE_2D, 0);
    texture_bind(renderer.emit, GL_TEXTURE_2D, 1);
    
    camera_isometric();
    camera_update(identity());
    r_map_draw();
    
    camera_orthographic();
    camera_update(identity());
    r_sprite_draw(gs);
  target_unbind();
  
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);
  
  shader_bind(renderer.blur);
  for (int i = 0; i < 4; i++) {
    glUniform1i(renderer.ul_horizontal, i % 2);
    
    target_bind(renderer.to_buffer[1]);
      texture_bind(renderer.bloom, GL_TEXTURE_2D, 0);
      vbuffer_draw(renderer.quad);
    target_unbind();
    
    target_bind(renderer.to_bloom);
      texture_bind(renderer.buffer[1], GL_TEXTURE_2D, 0);
      vbuffer_draw(renderer.quad);
    target_unbind();
  }
  
  target_bind(renderer.to_buffer[1]);
    texture_bind(renderer.buffer[0], GL_TEXTURE_2D, 0);
    texture_bind(renderer.bloom, GL_TEXTURE_2D, 1);
    shader_bind(renderer.shroud);
    vbuffer_draw(renderer.quad);
  target_unbind();
  
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  texture_bind(renderer.buffer[1], GL_TEXTURE_2D, 0);
  shader_bind(renderer.dither);
  vbuffer_draw(renderer.quad);
  
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
}

void renderer_load_map(map_t map)
{
  r_map_load(map);
}

void renderer_pipeline_init()
{
  renderer.sheet = texture_load_image("assets/sheet/color.png");
  renderer.emit = texture_load_image("assets/sheet/emit.png");
  
  renderer.depth = texture_create(VIEW_WIDTH, VIEW_HEIGHT, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
  renderer.bloom = texture_create(VIEW_WIDTH, VIEW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE);
  renderer.to_bloom = target_create(2, GL_COLOR_ATTACHMENT0, renderer.bloom, GL_DEPTH_ATTACHMENT, renderer.depth);
  
  for (int i = 0; i < 2; i++) {
    renderer.buffer[i] = texture_create(VIEW_WIDTH, VIEW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE);
    renderer.to_buffer[i] = target_create(
      2,
      GL_COLOR_ATTACHMENT0, renderer.buffer[i],
      GL_DEPTH_ATTACHMENT, renderer.depth
    );
  }
  
  renderer.to_buffer_and_bloom = target_create(
    3,
    GL_COLOR_ATTACHMENT0, renderer.buffer[0],
    GL_COLOR_ATTACHMENT1, renderer.bloom,
    GL_DEPTH_ATTACHMENT, renderer.depth
  );
    
  renderer.shroud = frame_shader_load("assets/shader/fragment/shroud.frag");
  renderer.dither = frame_shader_load("assets/shader/fragment/dither.frag");
  renderer.blur = frame_shader_load("assets/shader/fragment/blur.frag");
  
  renderer.ul_horizontal = glGetUniformLocation(renderer.blur, "horizontal");
  
  shader_bind(renderer.shroud);
  glUniform1i(glGetUniformLocation(renderer.shroud, "bloom"), 1);
  
  meshdata_t md = meshdata_create();
    meshdata_add_quad(md, identity(), identity());
    renderer.quad = vbuffer_add(md);
  meshdata_destroy(md);
}

void renderer_deinit()
{
  r_map_deinit();
  r_sprite_deinit();
  target_destroy(renderer.to_bloom);
  target_destroy(renderer.to_buffer[0]);
  target_destroy(renderer.to_buffer[1]);
  texture_destroy(renderer.bloom);
  texture_destroy(renderer.buffer[0]);
  texture_destroy(renderer.buffer[1]);
  texture_destroy(renderer.depth);
  texture_destroy(renderer.sheet);
  shader_destroy(renderer.shroud);
  shader_destroy(renderer.dither);
  shader_destroy(renderer.blur);
  vbuffer_deinit();
  camera_deinit();
}

shader_t frame_shader_load(const char *path)
{
  shaderdata_t sd = shaderdata_create();
    shaderdata_source(sd, "assets/shader/vertex/none.vert", SD_VERT);
    shaderdata_source(sd, path, SD_FRAG);
    shader_t shader = shader_load(sd);
  shaderdata_destroy(sd);
  return shader;
}
