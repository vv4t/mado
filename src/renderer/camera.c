#include "camera.h"

void camera_init_iso(camera_t *camera, float width, float height)
{
  camera->proj_mat = mat4x4_init_isometric(
    -width * camera->aspect_ratio,
    +width * camera->aspect_ratio,
    +height,
    -height,
    camera->near,
    camera->far
  );
}

mat4x4_t camera_setup_view(camera_t *camera)
{
  vec3_t view_pos = vec3_mulf(camera->pos, -1.0f);
  quat_t view_rot = quat_conjugate(camera->rot);
  
  mat4x4_t pos_mat = mat4x4_init_translation(view_pos);
  mat4x4_t rot_mat = mat4x4_init_rotation(view_rot);
  
  mat4x4_t view_mat = mat4x4_mul(pos_mat, rot_mat);
  
  camera->view_proj_mat = mat4x4_mul(view_mat, camera->proj_mat);
}

mat4x4_t camera_calc_mvp(const camera_t *camera, mat4x4_t model_mat)
{
  return mat4x4_mul(model_mat, camera->proj_mat);
}
