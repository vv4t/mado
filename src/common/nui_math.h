#ifndef NUI_MATH_H
#define NUI_MATH_H

#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>

typedef struct {
  float x;
  float y;
} vec2_t;

typedef struct {
  float x;
  float y;
  float z;
} vec3_t;

typedef struct {
  float x;
  float y;
  float z;
  float w;
} vec4_t;

typedef struct {
  float x;
  float y;
  float z;
  float w;
} quat_t;

typedef struct {
  vec3_t normal;
  float distance;
} plane_t;

typedef struct {
  float m[9];
} mat3x3_t;

typedef struct {
  float m[16];
} mat4x4_t;

inline vec2_t vec2_init(float x, float y)
{
  return (vec2_t) {
    .x = x,
    .y = y
  };
}

inline vec2_t vec2_add(vec2_t a, vec2_t b)
{
  return vec2_init(
    a.x + b.x,
    a.y + b.y);
}

inline vec2_t vec2_sub(vec2_t a, vec2_t b)
{
  return vec2_init(
    a.x - b.x,
    a.y - b.y);
}

inline vec2_t vec2_mul(vec2_t a, vec2_t b)
{
  return vec2_init(
    a.x * b.x,
    a.y * b.y);
}

inline vec2_t vec2_mulf(vec2_t a, float b)
{
  return vec2_init(
    a.x * b,
    a.y * b);
}

inline vec2_t vec2_rotate(vec2_t v, float theta)
{
  float c = cos(theta);
  float s = sin(theta);
  
  return vec2_init(
    v.x * c - v.y * s,
    v.x * s + v.y * c
  );
}

inline float vec2_dot(vec2_t a, vec2_t b)
{
  return a.x * b.x + a.y * b.y;
}

inline float vec2_length(vec2_t v)
{
  return sqrt(vec2_dot(v, v));
}

inline vec2_t vec2_normalize(vec2_t v)
{
  float length = vec2_length(v);

  if (length < 0.001f) {
    return vec2_init(0.0f, 0.0f);
  }

  return vec2_mulf(v, 1.0 / length);
}

inline vec3_t vec3_init(float x, float y, float z)
{
  return (vec3_t) {
    .x = x,
    .y = y,
    .z = z
  };
}

inline vec3_t vec3_add(vec3_t a, vec3_t b)
{
  return vec3_init(
    a.x + b.x,
    a.y + b.y,
    a.z + b.z);
}

inline vec3_t vec3_sub(vec3_t a, vec3_t b)
{
  return vec3_init(
    a.x - b.x,
    a.y - b.y,
    a.z - b.z);
}

inline vec3_t vec3_mulf(vec3_t a, float b)
{
  return vec3_init(
    a.x * b,
    a.y * b,
    a.z * b);
}

inline vec3_t vec3_mul(vec3_t a, vec3_t b)
{
  return vec3_init(
    a.x * b.x,
    a.y * b.y,
    a.z * b.z);
}

inline vec3_t vec3_cross(vec3_t a, vec3_t b)
{
  return vec3_init(
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x);
}

inline float vec3_dot(vec3_t a, vec3_t b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float vec3_length(vec3_t v)
{
  return sqrt(vec3_dot(v, v));
}

inline vec3_t vec3_normalize(vec3_t v)
{
  float length = vec3_length(v);
  
  if (length < 0.001f) {
    return vec3_init(0.0f, 0.0f, 0.0f);
  }
  
  return vec3_mulf(v, 1.0 / length);
}

inline vec4_t vec4_init(float x, float y, float z, float w)
{
  return (vec4_t) {
    .x = x,
    .y = y,
    .z = z,
    .w = w
  };
}

inline quat_t quat_init(float x, float y, float z, float w)
{
  return (quat_t) {
    .x = x,
    .y = y,
    .z = z,
    .w = w
  };
}

inline quat_t quat_conjugate(quat_t q)
{
  return quat_init(-q.x, -q.y, -q.z, q.w);
}

inline quat_t quat_init_rotation(vec3_t axis, float theta)
{
	float cos_half_angle = cos(theta / 2.0f);
	float sin_half_angle = sin(theta / 2.0f);
	
	float x = axis.x * sin_half_angle;
	float y = axis.y * sin_half_angle;
	float z = axis.z * sin_half_angle;
	float w = cos_half_angle;
  
  return quat_init(x, y, z, w);
}

inline quat_t quat_mul(quat_t a, quat_t b)
{
  return quat_init(
    a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
    a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
    a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
    a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
}

inline quat_t quat_mul_vec3(quat_t a, vec3_t b)
{
  return quat_init(
     a.w * b.x + a.y * b.z - a.z * b.y,
     a.w * b.y + a.z * b.x - a.x * b.z,
     a.w * b.z + a.x * b.y - a.y * b.x,
    -a.x * b.x - a.y * b.y - a.z * b.z);
}

inline vec3_t vec3_rotate(vec3_t v, quat_t q)
{
  quat_t q_conjugate = quat_conjugate(q);
  
  quat_t half_rot = quat_mul_vec3(q, v);
  quat_t full_rot = quat_mul(half_rot, q_conjugate);
  
  return vec3_init(full_rot.x, full_rot.y, full_rot.z);
}

inline mat3x3_t mat3x3_init(vec3_t a, vec3_t b, vec3_t c)
{
  mat3x3_t m;
  m.m[0]  = a.x;  m.m[3]  = a.y;  m.m[6] = a.z;
  m.m[1]  = b.x;  m.m[4]  = b.y;  m.m[7] = b.z;
  m.m[2]  = c.x;  m.m[5]  = c.y;  m.m[8] = c.z;
  return m;
}

inline mat3x3_t mat3x3_init_identity()
{
  mat3x3_t m;
  m.m[0]  = 1;  m.m[3]  = 0;  m.m[6] = 0;
  m.m[1]  = 0;  m.m[4]  = 1;  m.m[7] = 0;
  m.m[2]  = 0;  m.m[5]  = 0;  m.m[8] = 1;
  return m;
}

inline mat3x3_t mat3x3_init_scale(vec2_t v)
{
  mat3x3_t m;
  m.m[0]  = v.x;  m.m[3]  = 0;    m.m[6] = 0;
  m.m[1]  = 0;    m.m[4]  = v.y;  m.m[7] = 0;
  m.m[2]  = 0;    m.m[5]  = 0;    m.m[8] = 1;
  return m;
}

inline mat3x3_t mat3x3_init_translation(vec2_t v)
{
  mat3x3_t m;
  m.m[0]  = 1;  m.m[3]  = 0;  m.m[6] = v.x;
  m.m[1]  = 0;  m.m[4]  = 1;  m.m[7] = v.y;
  m.m[2]  = 0;  m.m[5]  = 0;  m.m[8] = 1;
  return m;
}

inline mat3x3_t mat3x3_mul(mat3x3_t a, mat3x3_t b)
{
  mat3x3_t m;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      m.m[i * 3 + j] =  b.m[0 * 3 + j] * a.m[i * 3 + 0] +
                        b.m[1 * 3 + j] * a.m[i * 3 + 1] +
                        b.m[2 * 3 + j] * a.m[i * 3 + 2];
                    
    }
  }
  
  return m;
}

inline vec2_t mat3x3_mul_vec2(mat3x3_t m, vec2_t v)
{
  return vec2_init(
    v.x * m.m[0] + v.y * m.m[3] + 1.0 * m.m[6],
    v.x * m.m[1] + v.y * m.m[4] + 1.0 * m.m[7]
  );
}

inline mat4x4_t mat4x4_init(vec4_t a, vec4_t b, vec4_t c, vec4_t d)
{
  mat4x4_t m;
  m.m[0]  = a.x;  m.m[4]  = a.y;  m.m[8]  = a.z;  m.m[12] = a.w;
  m.m[1]  = b.x;  m.m[5]  = b.y;  m.m[9]  = b.z;  m.m[13] = b.w;
  m.m[2]  = c.x;  m.m[6]  = c.y;  m.m[10] = c.z;  m.m[14] = c.w;
  m.m[3]  = d.x;  m.m[7]  = d.y;  m.m[11] = d.z;  m.m[15] = d.w;
  return m;
}

inline mat4x4_t mat4x4_init_identity()
{
  mat4x4_t m;
  m.m[0]  = 1;  m.m[4]  = 0;  m.m[8]  = 0;  m.m[12] = 0;
  m.m[1]  = 0;  m.m[5]  = 1;  m.m[9]  = 0;  m.m[13] = 0;
  m.m[2]  = 0;  m.m[6]  = 0;  m.m[10] = 1;  m.m[14] = 0;
  m.m[3]  = 0;  m.m[7]  = 0;  m.m[11] = 0;  m.m[15] = 1;
  return m;
}

inline mat4x4_t mat4x4_init_translation(vec3_t v)
{
  mat4x4_t m;
  m.m[0]  = 1;  m.m[4]  = 0;  m.m[8]  = 0;  m.m[12] = v.x;
  m.m[1]  = 0;  m.m[5]  = 1;  m.m[9]  = 0;  m.m[13] = v.y;
  m.m[2]  = 0;  m.m[6]  = 0;  m.m[10] = 1;  m.m[14] = v.z;
  m.m[3]  = 0;  m.m[7]  = 0;  m.m[11] = 0;  m.m[15] = 1;
  return m;
}

inline mat4x4_t mat4x4_init_scale(vec3_t v)
{
  mat4x4_t m;
  m.m[0]  = v.x;  m.m[4]  = 0;    m.m[8]  = 0;    m.m[12] = 0;
  m.m[1]  = 0;    m.m[5]  = v.y;  m.m[9]  = 0;    m.m[13] = 0;
  m.m[2]  = 0;    m.m[6]  = 0;    m.m[10] = v.z;  m.m[14] = 0;
  m.m[3]  = 0;    m.m[7]  = 0;    m.m[11] = 0;    m.m[15] = 1;
  return m;
}

inline mat4x4_t mat4x4_init_rotation(quat_t q)
{
  mat4x4_t m;
  m.m[0]  = 1-2*q.y*q.y - 2*q.z*q.z;  m.m[4]  = 2*q.x*q.y - 2*q.z*q.w;    m.m[8]  = 2*q.x*q.z + 2*q.y*q.w;    m.m[12] = 0;
  m.m[1]  = 2*q.x*q.y + 2*q.z*q.w;    m.m[5]  = 1-2*q.x*q.x - 2*q.z*q.z;  m.m[9]  = 2*q.y*q.z - 2*q.x*q.w;    m.m[13] = 0;
  m.m[2]  = 2*q.x*q.z - 2*q.y*q.w;    m.m[6]  = 2*q.y*q.z + 2*q.x*q.w;    m.m[10] = 1-2*q.x*q.x - 2 *q.y*q.y; m.m[14] = 0;
  m.m[3]  = 0;                        m.m[7]  = 0;                        m.m[11] = 0;                        m.m[15] = 1;
  
  return m;
}

inline mat4x4_t mat4x4_init_rotation_z(float theta)
{
  float cs = cos(theta);
  float sn = sin(theta);
  
  mat4x4_t m;
  m.m[0]  = cs;   m.m[4]  = -sn;  m.m[8]  = 0;    m.m[12] = 0;
  m.m[1]  = sn;   m.m[5]  = cs;   m.m[9]  = 0;    m.m[13] = 0;
  m.m[2]  = 0;    m.m[6]  = 0;    m.m[10] = 1;    m.m[14] = 0;
  m.m[3]  = 0;    m.m[7]  = 0;    m.m[11] = 0;    m.m[15] = 1;
  return m;
}

inline mat4x4_t mat4x4_mul(mat4x4_t a, mat4x4_t b)
{
  mat4x4_t m;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      m.m[i * 4 + j] =  b.m[0 * 4 + j] * a.m[i * 4 + 0] +
                        b.m[1 * 4 + j] * a.m[i * 4 + 1] +
                        b.m[2 * 4 + j] * a.m[i * 4 + 2] +
                        b.m[3 * 4 + j] * a.m[i * 4 + 3];
                    
    }
  }
  
  return m;
}

inline vec3_t mat4x4_mul_vec3(mat4x4_t m, vec3_t v)
{
  return vec3_init(
    v.x * m.m[0] + v.y * m.m[4] + v.z * m.m[8]  + 1.0 * m.m[12],
    v.x * m.m[1] + v.y * m.m[5] + v.z * m.m[9]  + 1.0 * m.m[13],
    v.x * m.m[2] + v.y * m.m[6] + v.z * m.m[10] + 1.0 * m.m[14]
  );
}

inline mat4x4_t mat4x4_init_transform(vec3_t translate, vec3_t scale)
{
  mat4x4_t translation_matrix = mat4x4_init_translation(translate);
  mat4x4_t scale_matrix = mat4x4_init_scale(scale);
  
  return mat4x4_mul(scale_matrix, translation_matrix);
}

inline mat4x4_t mat4x4_init_look_at(vec3_t at, vec3_t from, vec3_t up)
{
  vec3_t z_axis = vec3_normalize(vec3_sub(at, from));
  vec3_t x_axis = vec3_normalize(vec3_cross(up, z_axis));
  vec3_t y_axis = vec3_normalize(vec3_cross(z_axis, x_axis));
  
  float x_eye = -vec3_dot(x_axis, from);
  float y_eye = -vec3_dot(y_axis, from);
  float z_eye = -vec3_dot(z_axis, from);
  
  mat4x4_t m;
  m.m[0]  = x_axis.x; m.m[4]  = y_axis.x; m.m[8]  = z_axis.x; m.m[12] = x_eye;
  m.m[1]  = x_axis.y; m.m[5]  = y_axis.y; m.m[9]  = z_axis.y; m.m[13] = y_eye;
  m.m[2]  = x_axis.z; m.m[6]  = y_axis.z; m.m[10] = z_axis.z; m.m[14] = z_eye;
  m.m[3]  = 0;        m.m[7]  = 0;        m.m[11] = 0;        m.m[15] = 1;
  return m;
}

inline mat4x4_t mat4x4_init_perspective(float aspect_ratio, float fov, float near, float far)
{
  float tan_fov = 1 / tan(fov / 2);
  float ar_tan_fov = aspect_ratio * tan_fov;
  
  float z_scale = (-near - far) / (near - far);
  float z_offset = (2 * far * near) / (near - far);
  
  mat4x4_t m;
  m.m[0]  = ar_tan_fov; m.m[4]  = 0;        m.m[8]  = 0;        m.m[12] = 0;
  m.m[1]  = 0;          m.m[5]  = tan_fov;  m.m[9]  = 0;        m.m[13] = 0;
  m.m[2]  = 0;          m.m[6]  = 0;        m.m[10] = z_scale;  m.m[14] = z_offset;
  m.m[3]  = 0;          m.m[7]  = 0;        m.m[11] = 1;        m.m[15] = 0;
  
  return m;
}

inline mat4x4_t mat4x4_init_isometric(float l, float r, float t, float b, float n, float f)
{
  mat4x4_t m;
  m.m[0]	= 2 / (r - l);	m.m[4]	= 0;			      m.m[8]	= 0;			      m.m[12]	= -(r + l) / (r - l);
  m.m[1]	= 0;			      m.m[5]	= 2 / (t - b);	m.m[9]	= -4 / (f - n); m.m[13]	= -(t + b) / (t - b);
  m.m[2]	= 0;			      m.m[6]	= 0;			      m.m[10]	=  4 / (f - n);	m.m[14]	=  (f + n) / (f - n);
  m.m[3]	= 0;			      m.m[7]	= 0;			      m.m[11]	= 0;			      m.m[15]	= 1;
  
  return m;
}

inline mat4x4_t mat4x4_init_orthogonal(float l, float r, float t, float b, float n, float f)
{
  mat4x4_t m;
  m.m[0]	= 2 / (r - l);	m.m[4]	= 0;			      m.m[8]	= 0;			      m.m[12]	= -(r + l) / (r - l);
  m.m[1]	= 0;			      m.m[5]	= 2 / (t - b);	m.m[9]	= 0;            m.m[13]	= -(t + b) / (t - b);
  m.m[2]	= 0;			      m.m[6]	= 0;			      m.m[10]	= -2 / (f - n);	m.m[14]	= -(f + n) / (f - n);
  m.m[3]	= 0;			      m.m[7]	= 0;			      m.m[11]	= 0;			      m.m[15]	= 1;
  
  return m;
}

inline void mat4x4_print(mat4x4_t m)
{
  printf("mat4x4_t(%f %f %f %f\n",  m.m[0], m.m[4], m.m[8],   m.m[12]);
  printf("         %f %f %f %f\n",  m.m[1], m.m[5], m.m[9],   m.m[13]);
  printf("         %f %f %f %f\n",  m.m[2], m.m[6], m.m[10],  m.m[14]);
  printf("         %f %f %f %f)\n", m.m[3], m.m[7], m.m[11],  m.m[15]);
}

inline void vec2_print(vec2_t v)
{
  printf("vec2_t(%f %f)\n", v.x, v.y);
}

inline void vec3_print(vec3_t v)
{
  printf("vec3_t(%f %f %f)\n", v.x, v.y, v.z);
}

inline float to_radians(float x)
{
  return x * M_PI / 180.0;
}

inline float to_degrees(float x)
{
  return x * 180.0 / M_PI;
}

#endif
