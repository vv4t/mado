layout(location = 0) in vec3 v_p;
layout(location = 1) in vec2 v_uv;

#define SPRITE_MAX 32

struct ub_sprite_t {
  mat4 T_p;
  mat4 T_uv;
};

layout (std140) uniform spritedata {
  ub_sprite_t sprite[SPRITE_MAX];
  int num_sprite;
};

out vec2 vs_uv;

void main()
{
  vs_uv = vec2(v_uv.x, 1.0 - v_uv.y);
  gl_Position = mvp * sprite[gl_InstanceID].T_p * vec4(v_p, 1.0);
}
