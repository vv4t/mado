#include <gfx/camera.h>
#include <GL/glew.h>

struct ub_camera {
  matrix model;
  matrix view;
  matrix project;
  matrix mvp;
  vector view_pos;
};

struct {
  matrix p;
  matrix v;
  vector view_pos;
  float w;
  float h;
  GLuint ubo;
} camera;

void camera_init(float w, float h)
{
  glGenBuffers(1, &camera.ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, camera.ubo);
  glBufferData(GL_UNIFORM_BUFFER, 512, NULL, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera.ubo);
  
  camera.w = w;
  camera.h = h;
  camera.p = identity();
  camera.v = identity();
  camera.view_pos = (vector) {0};
}

void camera_shader_attach(shader_t shader)
{
  shader_bind(shader);
  glUniformBlockBinding(shader, glGetUniformBlockIndex(shader, "ub_camera"), 0);
}

void camera_shader_import(shaderdata_t sd)
{
  shaderdata_source(sd, "assets/shader/import/camera.glsl", SD_VERT | SD_FRAG);
}

void camera_orthographic()
{
  camera.p = scale(vec3(camera.w, camera.h, camera.w));
}

void camera_isometric()
{
  float w = camera.w;
  float h = camera.h;
  
  camera.p = mat4(
    vec4(w, 0, 0, 0),
    vec4(0, h,-w, 0),
    vec4(0, 0, w, 0),
    vec4(0, 0, 0, 1)
  );
}

void camera_move(vector position, float rotation)
{
  vector view_offset = fdotv(-1.0, position);
  float view_angle = -rotation;
  camera.view_pos = position;
  camera.v = mdotm(translate(view_offset), rotate_z(view_angle));
}

void camera_update(matrix model)
{
  struct ub_camera ub_camera = {
    .model = model,
    .view = camera.v,
    .project = camera.p,
    .mvp = mdotm(model, mdotm(camera.v, camera.p)),
    .view_pos = camera.view_pos
  };
  
  glBindBuffer(GL_UNIFORM_BUFFER, camera.ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ub_camera), &ub_camera);
}

void camera_deinit()
{
  glDeleteBuffers(1, &camera.ubo);
}

matrix camera_get_view() {
  return camera.v;
}
