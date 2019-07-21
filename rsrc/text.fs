#version 330

in vec2 texcoords;

uniform sampler2D symbol_atlas;
uniform vec3 color;

out vec4 FragColor;

void main()
{  
  FragColor = vec4(color, texture(symbol_atlas, texcoords).r);
}
