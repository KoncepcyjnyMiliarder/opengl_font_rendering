#version 330

in vec2 texCoords;

uniform sampler2D samplerTex;

out vec4 frag_color;

void main()
{
  frag_color = texture(samplerTex, texCoords).rgba;
}