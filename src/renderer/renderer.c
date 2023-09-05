#include "renderer.h"

#include "map_mesh.h"
#include "sprite_mesh.h"
#include "health_mesh.h"
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
  
  renderer->camera.pos = vec3_init(0.0, 0.0, 0.0);
  renderer->camera.rot = 0.0;
  
  return true;
}

void renderer_render(renderer_t *renderer, const game_t *game)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  renderer->camera.pos.x = game->cdict.transform[game->player].position.x;
  renderer->camera.pos.y = game->cdict.transform[game->player].position.y;
  renderer->camera.rot = game->cdict.transform[game->player].rotation;
  
  camera_set_isometric(&renderer->camera);
  camera_setup_view(&renderer->camera);
  glUniformMatrix4fv(renderer->ul_mvp, 1, GL_FALSE, renderer->camera.view_proj_mat.m);
  
  glDrawArrays(GL_TRIANGLES, renderer->map_mesh.offset, renderer->map_mesh.count);
  
  camera_set_orthogonal(&renderer->camera);
  camera_setup_view(&renderer->camera);
  glUniformMatrix4fv(renderer->ul_mvp, 1, GL_FALSE, renderer->camera.view_proj_mat.m);
  
  sprite_mesh_draw(&renderer->sprite_mesh, game, &renderer->camera);
  health_mesh_draw(&renderer->health_mesh, game, &renderer->camera);
}

void renderer_load_map(renderer_t *renderer, const map_t *map)
{
  map_mesh_init(&renderer->map_mesh, &renderer->buffer, map);
}

void renderer_load_sheet(renderer_t *renderer, const sprite_sheet_t *sprite_sheet)
{
  texture_load(&renderer->sprite_sheet, "assets/texture/texture.png");
  
  glUniform2f(
    renderer->ul_size_uv,
    1.0 / sprite_sheet->sheet_width,
    1.0 / sprite_sheet->sheet_height
  );
}

void renderer_init_camera(renderer_t *renderer)
{
  camera_viewport(&renderer->camera, 1280.0 / 720.0, 5.0, 5.0, -10.0, 10.0);
}

void renderer_init_gl(renderer_t *renderer)
{
  glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glUseProgram(renderer->program);
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, renderer->sprite_sheet);
}

bool renderer_init_mesh(renderer_t *renderer)
{
  sprite_mesh_init(&renderer->sprite_mesh, &renderer->buffer);
  health_mesh_init(&renderer->health_mesh, &renderer->buffer);
}

bool renderer_init_shader(renderer_t *renderer)
{
  char *shader_vsh = file_read_all("assets/shader/shader.vsh");
  char *shader_fsh = file_read_all("assets/shader/shader.fsh");
  
  shader_load(&renderer->program, "", shader_vsh, shader_fsh);
  glUseProgram(renderer->program);
  
  renderer->ul_mvp = glGetUniformLocation(renderer->program, "u_mvp");
  renderer->ul_size_uv = glGetUniformLocation(renderer->program, "u_size_uv");
  
  GLuint ul_texture = glGetUniformLocation(renderer->program, "u_texture");
  
  glUniform1i(ul_texture, 0);
  
  free(shader_vsh);
  free(shader_fsh);
  
  return true;
}
