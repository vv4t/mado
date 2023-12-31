layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec2 v_uv;

uniform mat4 u_mvp;
uniform vec2 u_size_uv;

out vec2 vs_uv;

void main()
{
  vs_uv = u_size_uv * v_uv;
  gl_Position = u_mvp * vec4(v_pos, 1.0);
}
