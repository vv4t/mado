layout (location = 0) out vec4 frag_color;

in vec2 vs_uv;
in vec4 vs_color;

void main()
{
  frag_color = vs_color;
}
