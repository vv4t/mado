#include "math3d.h"

#include <stdio.h>

void vector_print(const char *string, vector x)
{
  printf("%s: <%.6f, %.6f, %.6f, %.6f>\n", string, x.x, x.y, x.z, x.w); 
}

void matrix_print(const char *string, matrix A)
{
  printf("%s: [\n", string);
  printf("  <%.6f, %.6f, %.6f, %.6f>\n", A.m[0][0], A.m[0][1], A.m[0][2], A.m[0][3]); 
  printf("  <%.6f, %.6f, %.6f, %.6f>\n", A.m[1][0], A.m[1][1], A.m[1][2], A.m[1][3]); 
  printf("  <%.6f, %.6f, %.6f, %.6f>\n", A.m[2][0], A.m[2][1], A.m[2][2], A.m[2][3]); 
  printf("  <%.6f, %.6f, %.6f, %.6f>\n", A.m[3][0], A.m[3][1], A.m[3][2], A.m[3][3]);
  printf("]\n");
}
