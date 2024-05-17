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
} r_map;

static void add_tile(meshdata_t md, int x, int y);
static void add_block(meshdata_t md, int x, int y);
static void r_map_load(meshdata_t md);

void r_map_init()
{
  meshdata_t md = meshdata_create();
    r_map_load(md);
    r_map.mesh = vbuffer_add(md);
  meshdata_destroy(md);
  
  shaderdata_t sd = shaderdata_create();
    camera_shader_import(sd);
    shaderdata_source(sd, "assets/shader/map/shader.vert", SD_VERT);
    shaderdata_source(sd, "assets/shader/map/shader.frag", SD_FRAG);
    r_map.shader = shader_load(sd);
    camera_shader_attach(r_map.shader);
  shaderdata_destroy(sd);
}

void r_map_draw()
{
  shader_bind(r_map.shader);
  vbuffer_draw(r_map.mesh);
}

void r_map_load(meshdata_t md)
{
  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 32; j++) {
      add_tile(md, i, j);
      
      if (i % 8 == 0 && j % 8 == 0) {
        add_block(md, i, j);
      }
    }
  }
}

void add_tile(meshdata_t md, int x, int y)
{
  matrix T_p = mdotm(fscale(0.5), translate(vec2(x, y)));
  matrix T_uv = mdotm(translate(vec2(1, 7)), fscale(1.0 / 8.0));
  meshdata_add_quad(md, T_p, T_uv);
}

void add_block(meshdata_t md, int x, int y)
{
  vector N[] = {
    vec3(+1,  0,  0),
    vec3(-1,  0,  0),
    vec3( 0, +1,  0),
    vec3( 0, -1,  0)
  };
  
  for (int i = 0; i < 4; i++) {
    vector T = vec3(0, 0, -1);
    vector B = cross(T, N[i]);
    
    matrix T_p = mat3(T, B, N[i]);
    T_p = mdotm(mdotm(rotate_z(-M_PI / 2.0), translate(vec3(0, 0, 1))), T_p);
    T_p = mdotm(T_p, translate(vec3(0, 0,-1)));
    T_p = mdotm(T_p, fscale(0.5));
    T_p = mdotm(T_p, translate(vec2(x, y)));
    matrix T_uv = mdotm(translate(vec2(0, 7)), fscale(1.0 / 8.0));
    meshdata_add_quad(md, T_p, T_uv);
  }
  
  matrix T_p = mdotm(fscale(0.5), translate(vec3(x, y, -1)));
  matrix T_uv = mdotm(translate(vec2(0, 7)), fscale(1.0 / 8.0));
  meshdata_add_quad(md, T_p, T_uv);
}

void r_map_deinit()
{
  shader_destroy(r_map.shader);
}
