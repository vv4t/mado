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

void r_map_init()
{
  meshdata_t md = meshdata_create();
    meshdata_add_quad(md, mdotm(fscale(100.0), translate(vec3(0.0, 0.0, 0.0))), fscale(100.0));
    r_map.mesh = vbuffer_add(md);
  meshdata_destroy(md);
  
  shaderdata_t sd = shaderdata_create();
    camera_shader_import(sd);
    shaderdata_source(sd, "assets/shader/map/shader.vert", SD_VERT);
    shaderdata_source(sd, "assets/shader/map/shader.frag", SD_FRAG);
    r_map.shader = shader_load(sd);
    camera_shader_attach(r_map.shader);
  shaderdata_destroy(sd);
  
  r_map.sheet = texture_load_image("assets/sheet/grass.png");
}

void r_map_draw()
{
  shader_bind(r_map.shader);
  texture_bind(r_map.sheet, GL_TEXTURE_2D, 0);
  vbuffer_draw(r_map.mesh);
}

void r_map_deinit()
{
  texture_destroy(r_map.sheet);
  shader_destroy(r_map.shader);
}
