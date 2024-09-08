layout(location = 0) in vec3 v_p;
layout(location = 1) in vec2 v_uv;

struct rect_t {
  vec2 xy;
  vec2 wh;
  vec2 txy;
  vec2 twh;
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
  
  vs_uv = (rect[gl_InstanceID].txy + v_uv * rect[gl_InstanceID].twh) / 16.0;
  vs_uv.y = 1.0 - vs_uv.y;
  
  gl_Position.xy = 2.0 * rect[gl_InstanceID].xy - 1.0 + (v_p.xy + 1.0) * rect[gl_InstanceID].wh * vec2(960.0 / 1280.0, 1.0);
  gl_Position.z = v_p.z;
  gl_Position.w = 1.0;
}
