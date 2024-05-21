#include <renderer/renderer.h>
#include <renderer/texture.h>
#include <renderer/shader.h>
#include <renderer/mesh.h>
#include <renderer/camera.h>
#include <renderer/r_sprite.h>
#include <renderer/r_map.h>
#include <stdio.h>
#include <GL/glew.h>

#define FOV (1.0 / 6.0)

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

#define ASPECT_RATIO ((float) SCR_WIDTH / (float) SCR_HEIGHT)

#define MAX_VERTICES 1024 * 1024

struct {
  mesh_t map;
} renderer;

static void renderer_init_assets();
static void renderer_draw_sprites(const game_t *gs);

void renderer_init()
{
  camera_init();
  vbuffer_init(MAX_VERTICES);
  vbuffer_bind();
  r_sprite_init();
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
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
  camera_isometric(FOV * 1.0, FOV * ASPECT_RATIO);
  camera_update(identity());
  r_map_draw();
  
  camera_orthographic(FOV * 1.0, FOV * ASPECT_RATIO);
  camera_update(identity());
  r_sprite_draw(gs);
}

void renderer_load_map(map_t map)
{
  r_map_load(map);
}

void renderer_deinit()
{
  r_map_deinit();
  r_sprite_deinit();
  vbuffer_deinit();
  camera_deinit();
}
