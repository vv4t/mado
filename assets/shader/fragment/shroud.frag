out vec4 frag_color;

in vec3 vs_p;
in vec2 vs_uv;

uniform sampler2D screen;
uniform sampler2D bloom;

void main()
{
  frag_color = texture(screen, vs_uv);
  
  vec3 d_p = vs_p * vec3(1280.0 / 960.0, 1.0, 1.0);
  frag_color.rgb *= 0.5 + clamp(0.2 / pow(dot(d_p, d_p), 2.0), 0.0, 0.5);
  frag_color.rgb += texture(bloom, vs_uv).xyz * 2.0;
  
  const float gamma = 0.4;
  const float exposure = 3.0;
  
  vec3 hdr_color = frag_color.rgb;
  
  vec3 mapped = vec3(1.0) - exp(-hdr_color * exposure);
  mapped = pow(mapped, vec3(1.0 / gamma));
  
  frag_color = vec4(mapped, 1.0);
}
