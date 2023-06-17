#ifndef CAMERA_H
#define CAMERA_H

#include "../common/nui_math.h"

typedef struct {
  float     aspect_ratio;
  float     near;
  float     far;
  
  vec3_t    pos;
  quat_t    rot;
  
  mat4x4_t  proj_mat;
  mat4x4_t  view_proj_mat;
} camera_t;

void      camera_init_iso(camera_t *camera, float width, float height);
mat4x4_t  camera_setup_view(camera_t *camera);
mat4x4_t  camera_calc_mvp(const camera_t *camera, mat4x4_t model);

#endif
