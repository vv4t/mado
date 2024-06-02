layout(location = 0) in vec3 v_p;
layout(location = 1) in vec2 v_uv;

struct rect_t {
  vec2 p_pos;
  vec2 p_size;
  vec2 uv_pos;
  vec2 uv_size;
  vec4 color;
};

layout (std140) uniform rectdata {
  rect_t rect[RECT_MAX];
};

out vec2 vs_uv;
out vec4 vs_color;

void main()
{
  vs_color = rect[gl_InstanceID].color;
  vs_uv = rect[gl_InstanceID].uv_pos + v_uv * rect[gl_InstanceID].uv_size;
  vs_uv = vec2(vs_uv.x, 1.0 - vs_uv.y);
  gl_Position.xy = 2.0 * rect[gl_InstanceID].p_pos + (v_p.xy + 1.0) * rect[gl_InstanceID].p_size - 1.0;
  gl_Position.z = v_p.z;
  gl_Position.w = 1.0;
}
