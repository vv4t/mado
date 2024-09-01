#ifndef CAMERA_H
#define CAMERA_H

#include <lib/math3d.h>
#include <gfx/shader.h>

void camera_init(float w, float h);

void camera_orthographic();
void camera_isometric();
void camera_move(vector position, float rotation);
void camera_update(matrix m);

void camera_shader_attach(shader_t shader);
void camera_shader_import(shaderdata_t sd);

void camera_deinit();

matrix camera_get_view();

#endif
