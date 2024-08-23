#include <gfx/map_renderer.h>
#include <gfx/r_def.h>
#include <gfx/camera.h>
#include <gfx/mesh.h>
#include <gfx/shader.h>
#include <gfx/texture.h>
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
} map_renderer;

static int skip_wall_check(map_t map, int x, int y, int z);
static void add_tile(meshdata_t md, int x, int y, int z, int tx, int ty);
static void add_block(meshdata_t md, int x, int y, int z, int skip, int tx, int ty);

void map_renderer_init()
{
  shaderdata_t sd = shaderdata_create();
    camera_shader_import(sd);
    shaderdata_source(sd, "assets/shader/vertex/mvp.vert", SD_VERT);
    shaderdata_source(sd, "assets/shader/fragment/world.frag", SD_FRAG);
    map_renderer.shader = shader_load(sd);
    camera_shader_attach(map_renderer.shader);
    glUniform1i(glGetUniformLocation(map_renderer.shader, "emit"), 1);
  shaderdata_destroy(sd);
}

void map_renderer_draw()
{
  shader_bind(map_renderer.shader);
  vbuffer_draw(map_renderer.mesh);
}

void map_renderer_load(map_t map)
{
  meshdata_t md = meshdata_create();
  
  for (int i = 0; i < map_get_width(map); i++) {
    for (int j = 0; j < map_get_height(map); j++) {
      tilename_t tilename = map_get(map, i, j);
      
      if (tilename == 0) {
        continue;
      }
      
      tile_t tile;
      tile_get(&tile, tilename);
      
      int z = 0;
      
      for (z = 0; z < tile.num_block; z++) {
        int skip = 0b0000;
        
        if (skip_wall_check(map, i + 1, j, z) > 0) skip |= SKIP_RIGHT;
        if (skip_wall_check(map, i - 1, j, z) > 0) skip |= SKIP_LEFT;
        if (skip_wall_check(map, i, j + 1, z) > 0) skip |= SKIP_UP;
        if (skip_wall_check(map, i, j - 1, z) > 0) skip |= SKIP_DOWN;
        
        add_block(md, i, j, z, skip, tile.block[z].tx, tile.block[z].ty);
      }
      
      add_tile(md, i, j, z, tile.tx, tile.ty);
    }
  }
  
  map_renderer.mesh = vbuffer_add(md);
  meshdata_destroy(md);
}

void map_renderer_deinit()
{
  shader_destroy(map_renderer.shader);
}

int skip_wall_check(map_t map, int x, int y, int z)
{
  tilename_t tilename = map_get(map, x, y);
  
  tile_t tile;
  tile_get(&tile, tilename);
  
  return tile.num_block > z;
}

void add_tile(meshdata_t md, int x, int y, int z, int tx, int ty)
{
  matrix T_p = mdotm(fscale(0.5), translate(vec3(x + 0.5, y + 0.5, -z)));
  matrix T_uv = mdotm(translate(vec2(tx, ty)), fscale(1.0 / SHEET_SIZE));
  meshdata_add_quad(md, T_p, T_uv);
}

void add_block(meshdata_t md, int x, int y, int z, int skip, int tx, int ty)
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
    T_p = mdotm(T_p, fscale(0.5));
    T_p = mdotm(T_p, translate(vec3(x + 0.5, y + 0.5, -z - 0.5)));
    matrix T_uv = mdotm(translate(vec2(tx, ty)), fscale(1.0 / SHEET_SIZE));
    meshdata_add_quad(md, T_p, T_uv);
  }
}
