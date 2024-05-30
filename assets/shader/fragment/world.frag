layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 frag_bright;

in vec2 vs_uv;

uniform sampler2D sheet;

void main()
{
  frag_color = texture(sheet, vs_uv);
  
  if (frag_color.w < 1.0) {
    discard;
  }
  
  float brightness = dot(frag_color.rgb, vec3(1.8, 0.2, 0.2));
  if (brightness > 0.5) frag_bright = vec4(frag_color.rgb, 1.0);
  else frag_bright = vec4(0.0, 0.0, 0.0, 1.0);
}
