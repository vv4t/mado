#ifndef MATH3D_H
#define MATH3D_H

#define _USE_MATH_DEFINES

#include <math.h>

typedef struct {
  float x;
  float y;
  float z;
  float w;
} vector;

typedef struct {
  float m[4][4];
} matrix;

void vector_print(const char *string, vector x);
void matrix_print(const char *string, matrix A);

inline static vector vec2(float x, float y) { return (vector) { .x = x, .y = y, .z = 0, .w = 0 }; }
inline static vector vec3(float x, float y, float z) { return (vector) { .x = x, .y = y, .z = z, .w = 0 }; }
inline static vector vec4(float x, float y, float z, float w) { return (vector) { .x = x, .y = y, .z = z, .w = w }; }

inline static vector v2pt(vector x) { return (vector) { .x = x.x, .y = x.y, .z = x.z, .w = 1.0 }; }

inline static matrix mat4(vector a, vector b, vector c, vector d)
{
  return (matrix) {
    .m[0][0] = a.x, .m[1][0] = a.y, .m[2][0] = a.z, .m[3][0] = a.w,
    .m[0][1] = b.x, .m[1][1] = b.y, .m[2][1] = b.z, .m[3][1] = b.w,
    .m[0][2] = c.x, .m[1][2] = c.y, .m[2][2] = c.z, .m[3][2] = c.w,
    .m[0][3] = d.x, .m[1][3] = d.y, .m[2][3] = d.z, .m[3][3] = d.w
  };
}

inline static matrix mat3(vector a, vector b, vector c)
{
  return (matrix) {
    .m[0][0] = a.x, .m[1][0] = b.x, .m[2][0] = c.x, .m[3][0] = 0.0,
    .m[0][1] = a.y, .m[1][1] = b.y, .m[2][1] = c.y, .m[3][1] = 0.0,
    .m[0][2] = a.z, .m[1][2] = b.z, .m[2][2] = c.z, .m[3][2] = 0.0,
    .m[0][3] = 0.0, .m[1][3] = 0.0, .m[2][3] = 0.0, .m[3][3] = 1.0
  };
}

inline static matrix mat3_from_mat4(matrix m)
{
  m.m[0][3] = 0.0;
  m.m[1][3] = 0.0;
  m.m[2][3] = 0.0;
  
  m.m[3][0] = 0.0;
  m.m[3][1] = 0.0;
  m.m[3][2] = 0.0;
  
  return m;
}

inline static vector vaddv(vector A, vector B)
{
  return (vector) {
    .x = A.x + B.x,
    .y = A.y + B.y,
    .z = A.z + B.z,
    .w = A.w + B.w
  };
}

inline static vector vsubv(vector A, vector B)
{
  return (vector) {
    .x = A.x - B.x,
    .y = A.y - B.y,
    .z = A.z - B.z,
    .w = A.w - B.w
  };
}

inline static vector fdotv(float a, vector x)
{
  return (vector) {
    .x = a * x.x,
    .y = a * x.y,
    .z = a * x.z,
    .w = a * x.w
  };
}

inline static vector mdotv(matrix A, vector x)
{
  return vec4(
    A.m[0][0] * x.x + A.m[1][0] * x.y + A.m[2][0] * x.z + A.m[3][0] * x.w,
    A.m[0][1] * x.x + A.m[1][1] * x.y + A.m[2][1] * x.z + A.m[3][1] * x.w,
    A.m[0][2] * x.x + A.m[1][2] * x.y + A.m[2][2] * x.z + A.m[3][2] * x.w,
    A.m[0][3] * x.x + A.m[1][3] * x.y + A.m[2][3] * x.z + A.m[3][3] * x.w
  );
}

inline static float dot(vector A, vector B)
{
  return A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w;
}

inline static float length(vector x)
{
  return sqrt(dot(x, x));
}

inline static vector normalize(vector x)
{
  return fdotv(1.0 / length(x), x);
}

inline static vector cross(vector a, vector b)
{
  return (vector) {
    .x = a.y * b.z - a.z * b.y,
    .y = a.z * b.x - a.x * b.z,
    .z = a.x * b.y - a.y * b.x,
    .w = 1.0
  };
}

inline static matrix mdotm(matrix A, matrix B)
{
  matrix R;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      R.m[i][j] = 0.0;
      
      for (int k = 0; k < 4; k++) {
        R.m[i][j] += A.m[i][k] * B.m[k][j];
      }
    }
  }
  
  return R;
}

inline static matrix identity()
{
  return mat4(
    vec4(1, 0, 0, 0),
    vec4(0, 1, 0, 0),
    vec4(0, 0, 1, 0),
    vec4(0, 0, 0, 1)
  );
}

inline static matrix translate(vector x)
{
  return mat4(
    vec4(1, 0, 0, x.x),
    vec4(0, 1, 0, x.y),
    vec4(0, 0, 1, x.z),
    vec4(0, 0, 0, 1)
  );
}

inline static matrix fscale(float a)
{
  return mat4(
    vec4(a, 0, 0, 0),
    vec4(0, a, 0, 0),
    vec4(0, 0, a, 0),
    vec4(0, 0, 0, 1)
  );
}

inline static matrix scale(vector x)
{
  return mat4(
    vec4(x.x, 0, 0, 0),
    vec4(0, x.y, 0, 0),
    vec4(0, 0, x.z, 0),
    vec4(0, 0, 0,   1)
  );
}

inline static matrix rotate_x(float t)
{
  float c = cos(t);
  float s = sin(t);
  
  return mat4(
    vec4(1,   0,    0,    0),
    vec4(0,   c,   -s,    0),
    vec4(0,   s,   +c,    0),
    vec4(0,   0,    0,    1)
  );
}

inline static matrix rotate_y(float t)
{
  float c = cos(t);
  float s = sin(t);
  
  return mat4(
    vec4(c,   0,    -s,   0),
    vec4(0,   1,    0,    0),
    vec4(s,   0,    +c,   0),
    vec4(0,   0,    0,    1)
  );
}

inline static matrix rotate_z(float t)
{
  float c = cos(t);
  float s = sin(t);
  
  return mat4(
    vec4(c,   -s,   0,    0),
    vec4(s,   +c,   0,    0),
    vec4(0,   0,    1,    0),
    vec4(0,   0,    0,    1)
  );
}

inline static matrix rotate_xyz(vector r)
{
  return mdotm(rotate_x(r.x), mdotm(rotate_y(r.y), rotate_z(r.z)));
}

inline static matrix rotate_zyx(vector r)
{
  return mdotm(rotate_z(r.z), mdotm(rotate_y(r.y), rotate_x(r.x)));
}

inline static matrix transform(vector t, vector r, vector s)
{
  return mdotm(scale(s), mdotm(rotate_xyz(r), translate(t)));
}

inline static matrix perspective(float w, float h, float n, float f)
{
  float u = (-f - n) / (-f + n);
  float v = (2 * f * n) / (-f + n);
  
  return mat4(
    vec4(w, 0, 0, 0),
    vec4(0, h, 0, 0),
    vec4(0, 0, u, v),
    vec4(0, 0, 1, 0)
  );
}

inline static matrix isometric(float w, float h)
{
  return mat4(
    vec4(w, 0,  0, 0),
    vec4(0, h, -1, 0),
    vec4(0, 0, +1, 0),
    vec4(0, 0,  0, 1)
  );
}

inline static matrix inverse(matrix A)
{
#define ROW_ADD(A, a, b, k) for (int z = 0; z < 4; z++) A.m[a][z] += A.m[b][z] * k;
#define ROW_MULF(A, a, k) for (int z = 0; z < 4; z++) A.m[a][z] *= k;
  matrix I = identity();
  
  for (int i = 0; i < 3; i++) {
    for (int j = 2; j >= i; j--) {
      if (A.m[j][i] == 0.0) continue;
      float ratio = (A.m[j + 1][i]) / A.m[j][i];
      ROW_ADD(A, j + 1, j, -ratio);
      ROW_ADD(I, j + 1, j, -ratio);
    }
  }
  
  for (int i = 0; i < 4; i++) {
    float ratio = 1.0 / A.m[i][i];
    ROW_MULF(A, i, ratio);
    ROW_MULF(I, i, ratio);
  }
  
  for (int i = 3; i >= 0; i--) {
    for (int j = 0; j < i; j++) {
      float ratio = A.m[j][i];
      ROW_ADD(A, j, i, -ratio);
      ROW_ADD(I, j, i, -ratio);
    }
  }
  
  return I;
}

#endif
