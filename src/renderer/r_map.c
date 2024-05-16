#include <renderer/r_map.h>
#include <renderer/camera.h>
#include <renderer/mesh.h>
#include <renderer/shader.h>
#include <renderer/texture.h>
#include <math3d.h>
#include <GL/glew.h>
#include <stdio.h>

struct {
  mesh_t mesh;
  shader_t shader;
  texture_t sheet;
} r_map;

static void map_load(meshdata_t md);

void r_map_init()
{
  meshdata_t md = meshdata_create();
    meshdata_add_quad(md, fscale(100.0), fscale(100.0));
    r_map.mesh = vbuffer_add(md);
  meshdata_destroy(md);
  
  shaderdata_t sd = shaderdata_create();
    camera_shader_import(sd);
    shaderdata_source(sd, "assets/shader/map/shader.vert", SD_VERT);
    shaderdata_source(sd, "assets/shader/map/shader.frag", SD_FRAG);
    r_map.shader = shader_load(sd);
    camera_shader_attach(r_map.shader);
  shaderdata_destroy(sd);
  
  r_map.sheet = texture_load_image("assets/sheet/grid.png");
}

void map_load(meshdata_t md)
{
  matrix T_uv = mdotm(translate(vec2(3.0, 7.0)), fscale(1.0 / 8.0));
  
  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 32; j++) {
      matrix T_p = mdotm(fscale(0.5), translate(vec2(i, j)));
      meshdata_add_quad(md, T_p, T_uv);
    }
  }
  
  matrix T_p = mdotm(mdotm(rotate_x(M_PI / 2.0), fscale(0.5)), translate(vec2(-3, -3)));
  meshdata_add_quad(md, T_p, T_uv);
}

void r_map_draw()
{
  shader_bind(r_map.shader);
  texture_bind(r_map.sheet, GL_TEXTURE_2D, 0);
  vbuffer_draw(r_map.mesh);
}

void r_map_deinit()
{
  shader_destroy(r_map.shader);
  texture_destroy(r_map.sheet);
}
