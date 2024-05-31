#include <renderer/r_map.h>
#include <renderer/r_def.h>
#include <renderer/camera.h>
#include <renderer/mesh.h>
#include <renderer/shader.h>
#include <renderer/texture.h>
#include <lib/math3d.h>
#include <GL/glew.h>
#include <stdio.h>

enum {
  SKIP_UP     = 0b0001,
  SKIP_DOWN   = 0b0010,
  SKIP_LEFT   = 0b0100,
  SKIP_RIGHT  = 0b1000
};

struct {
  mesh_t mesh;
  shader_t shader;
} r_map;

static void add_tile(meshdata_t md, int x, int y);
static void add_block(meshdata_t md, int x, int y, int wall);

void r_map_init()
{
  shaderdata_t sd = shaderdata_create();
    camera_shader_import(sd);
    shaderdata_source(sd, "assets/shader/vertex/mvp.vert", SD_VERT);
    shaderdata_source(sd, "assets/shader/fragment/world.frag", SD_FRAG);
    r_map.shader = shader_load(sd);
    camera_shader_attach(r_map.shader);
    glUniform1i(glGetUniformLocation(r_map.shader, "emit"), 1);
  shaderdata_destroy(sd);
}

void r_map_draw()
{
  shader_bind(r_map.shader);
  vbuffer_draw(r_map.mesh);
}

void r_map_load(map_t map)
{
  meshdata_t md = meshdata_create();
  
  for (int i = 0; i < map_get_width(map); i++) {
    for (int j = 0; j < map_get_height(map); j++) {
      if (map_get(map, i, j) > 0) {
        int skip = 0b0000;
        
        if (map_get(map, i + 1, j) > 0) skip |= SKIP_RIGHT;
        if (map_get(map, i - 1, j) > 0) skip |= SKIP_LEFT;
        if (map_get(map, i, j + 1) > 0) skip |= SKIP_UP;
        if (map_get(map, i, j - 1) > 0) skip |= SKIP_DOWN;
        
        add_block(md, i, j, skip);
      } else {
        add_tile(md, i, j);
      }
    }
  }
  
  r_map.mesh = vbuffer_add(md);
  meshdata_destroy(md);
}

void add_tile(meshdata_t md, int x, int y)
{
  matrix T_p = mdotm(fscale(0.5), translate(vec2(x + 0.5, y + 0.5)));
  matrix T_uv = mdotm(translate(vec2(1, 7)), fscale(1.0 / SHEET_SIZE));
  meshdata_add_quad(md, T_p, T_uv);
}

void add_block(meshdata_t md, int x, int y, int skip)
{
  vector N[] = {
    vec3(+1,  0,  0),
    vec3(-1,  0,  0),
    vec3( 0, +1,  0),
    vec3( 0, -1,  0)
  };
  
  int check[] = {
    SKIP_RIGHT,
    SKIP_LEFT,
    SKIP_UP,
    SKIP_DOWN
  };
  
  for (int i = 0; i < 4; i++) {
    if ((skip & check[i]) > 0) {
      continue;
    }
    
    vector T = vec3(0, 0, -1);
    vector B = cross(T, N[i]);
    
    matrix T_p = mat3(T, B, N[i]);
    T_p = mdotm(mdotm(rotate_z(-M_PI / 2.0), translate(vec3(0, 0, 1))), T_p);
    T_p = mdotm(T_p, translate(vec3(0, 0,-1)));
    T_p = mdotm(T_p, fscale(0.5));
    T_p = mdotm(T_p, translate(vec2(x + 0.5, y + 0.5)));
    matrix T_uv = mdotm(translate(vec2(0, 7)), fscale(1.0 / SHEET_SIZE));
    meshdata_add_quad(md, T_p, T_uv);
  }
  
  matrix T_p = mdotm(fscale(0.5), translate(vec3(x + 0.5, y + 0.5, -1)));
  matrix T_uv = mdotm(translate(vec2(0, 7)), fscale(1.0 / SHEET_SIZE));
  meshdata_add_quad(md, T_p, T_uv);
}

void r_map_deinit()
{
  shader_destroy(r_map.shader);
}
