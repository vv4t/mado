out vec4 frag_color;

in vec2 vs_uv;
in vec4 vs_color;

uniform sampler2D sheet;

void main()
{
  frag_color = texture(sheet, vs_uv) * vs_color;
}
