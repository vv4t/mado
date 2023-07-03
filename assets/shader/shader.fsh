out vec4 frag_color;

in vec2 vs_uv;

uniform sampler2D u_texture;

void main()
{
  vec4 color = texture(u_texture, vs_uv);
  
  if (color.a == 0.0)
    discard;
  
  frag_color = color;
}
