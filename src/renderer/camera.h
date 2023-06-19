#ifndef CAMERA_H
#define CAMERA_H

#include "../common/nui_math.h"

typedef struct {
  float     aspect_ratio;
  float     near;
  float     far;
  float     width;
  float     height;
  
  vec3_t    pos;
  quat_t    rot;
  
  mat4x4_t  proj_mat;
  mat4x4_t  view_proj_mat;
} camera_t;

void      camera_set_orthogonal(camera_t *camera);
void      camera_set_isometric(camera_t *camera);
void      camera_setup_view(camera_t *camera);
mat4x4_t  camera_calc_mvp(const camera_t *camera, mat4x4_t model);

#endif
