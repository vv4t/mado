layout(location = 0) in vec3 v_p;
layout(location = 1) in vec2 v_uv;

struct ub_sprite_t {
  mat4 T_p;
  mat4 T_uv;
};

layout (std140) uniform spritedata {
  ub_sprite_t sprite[INSTANCED_MAX];
};

out vec2 vs_uv;

void main()
{
  vs_uv = vec2(sprite[gl_InstanceID].T_uv * vec4(v_uv, 0.0, 1.0));
  vs_uv = vec2(vs_uv.x, 1.0 - vs_uv.y);
  gl_Position = mvp * sprite[gl_InstanceID].T_p * vec4(v_p, 1.0);
}
