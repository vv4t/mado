#include <renderer/renderer.h>
#include <renderer/texture.h>
#include <renderer/shader.h>
#include <renderer/mesh.h>
#include <renderer/camera.h>
#include <renderer/r_sprite.h>
#include <renderer/r_map.h>
#include <stdio.h>
#include <GL/glew.h>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

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
  
  float fov = 1.0 / 6.0;
  float aspect_ratio = (float) SCR_WIDTH / (float) SCR_HEIGHT;
  camera_isometric(fov * 1.0, fov * aspect_ratio);
}

float t = 0.0;

void renderer_render(const game_t *gs)
{
  t += 0.015;
  
  const transform_t *pt = ENTITY_GET_COMPONENT(gs->edict, gs->player, transform);
  camera_move(pt->position, pt->rotation);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera_update(identity());
  r_map_draw();
  r_sprite_draw(gs);
}

void renderer_deinit()
{
  r_map_deinit();
  r_sprite_deinit();
  vbuffer_deinit();
  camera_deinit();
}
