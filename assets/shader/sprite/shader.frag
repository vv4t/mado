out vec4 frag_color;

in vec2 vs_uv;

uniform sampler2D u_sprite;

void main()
{
  frag_color = texture(u_sprite, vs_uv);
  if (frag_color.w < 1.0) discard;
}
