#include "renderer.h"

#include "map_mesh.h"
#include "texture.h"
#include "../common/log.h"
#include "../common/file.h"
#include <stdlib.h>

void renderer_init_camera(renderer_t *renderer);
bool renderer_init_shader(renderer_t *renderer);
bool renderer_init_mesh(renderer_t *renderer);
void renderer_init_gl(renderer_t *renderer);

bool renderer_init(renderer_t *renderer)
{
  buffer_init(&renderer->buffer, 32 * 1024);
  
  renderer_init_camera(renderer);
  renderer_init_shader(renderer);
  renderer_init_mesh(renderer);
  renderer_init_gl(renderer);
  
  texture_load(&renderer->texture, "assets/texture/texture.png");
  
  renderer->camera.rot = quat_init(0.0, 0.0, 0.0, 1.0);
  
  return true;
}

void renderer_render(renderer_t *renderer)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  renderer->camera.rot = quat_mul(quat_init_rotation(vec3_init(0.0, 0.0, 1.0), 0.01), renderer->camera.rot);
  
  camera_setup_view(&renderer->camera);
  
  glUniformMatrix4fv(renderer->ul_mvp, 1, GL_FALSE, renderer->camera.view_proj_mat.m);
  
  glDrawArrays(GL_TRIANGLES, renderer->mesh.offset, renderer->mesh.count);
}

void renderer_init_camera(renderer_t *renderer)
{
  renderer->camera.aspect_ratio = 1280.0 / 720.0;
  renderer->camera.near = -10.0;
  renderer->camera.far = 10.0;
  
  camera_init_iso(&renderer->camera, 5.0, 5.0);
}

void renderer_init_gl(renderer_t *renderer)
{
  glClearColor(0.0f, 0.4f, 1.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  
  glUseProgram(renderer->program);
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, renderer->texture);
}

bool renderer_init_mesh(renderer_t *renderer)
{
  map_mesh_init(&renderer->mesh, &renderer->buffer);
}

bool renderer_init_shader(renderer_t *renderer)
{
  char *shader_vsh = file_read_all("assets/shader/shader.vsh");
  char *shader_fsh = file_read_all("assets/shader/shader.fsh");
  
  shader_load(&renderer->program, "", shader_vsh, shader_fsh);
  glUseProgram(renderer->program);
  
  renderer->ul_mvp = glGetUniformLocation(renderer->program, "u_mvp");
  
  GLuint ul_texture = glGetUniformLocation(renderer->program, "u_texture");
  glUniform1i(ul_texture, 0);
  
  free(shader_vsh);
  free(shader_fsh);
  
  return true;
}
