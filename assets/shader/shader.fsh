out vec4 frag_color;

in vec2 vs_uv;

uniform sampler2D u_texture;

void main()
{
  frag_color = texture(u_texture, vs_uv);
}
