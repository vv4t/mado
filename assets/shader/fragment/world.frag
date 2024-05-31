layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 frag_bright;

in vec2 vs_uv;

uniform sampler2D sheet;
uniform sampler2D emit;

void main()
{
  frag_color = texture(sheet, vs_uv);
  frag_bright = texture(emit, vs_uv);
  
  if (frag_color.w < 1.0) {
    discard;
  }
}
