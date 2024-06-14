layout(location = 0) in vec3 v_p;
layout(location = 1) in vec2 v_uv;

struct rect_t {
  float x, y, w, h;
  float tx, ty, tw, th;
  vec4 color;
};

layout (std140) uniform ubo_gui {
  rect_t rectdata[RECT_MAX];
};

out vec2 vs_uv;
out vec4 vs_color;

void main()
{
  rect_t rect = rectdata[gl_InstanceID];
  
  vs_color = rect.color;
  
  vs_uv = vec2(rect.tx, rect.ty) + v_uv * vec2(rect.tw, rect.th);
  vs_uv *= (1.0 / 16.0);
  vs_uv = vec2(vs_uv.x, 1.0 - vs_uv.y);
  
  gl_Position.xy = 2.0 * vec2(rect.x, 1.0 - rect.y - rect.h);
  gl_Position.xy += (v_p.xy + 1.0) * vec2(rect.w, rect.h) * vec2(960.0 / 1280.0, 1.0);
  gl_Position.xy -= 1.0;
  gl_Position.z = v_p.z;
  gl_Position.w = 1.0;
}
