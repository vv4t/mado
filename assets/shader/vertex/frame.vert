layout(location = 0) in vec3 v_p;
layout(location = 1) in vec2 v_uv;

out vec3 vs_p;
out vec2 vs_uv;

void main()
{
  vs_p = v_p;
  vs_uv = v_uv;
  gl_Position = vec4(v_p, 1.0);
}
