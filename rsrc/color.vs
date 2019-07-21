#version 330

layout (location = 0) in vec3 loc;
layout (location = 1) in vec2 uv;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec2 texCoords;

void main()
{
  gl_Position = proj * view * model * vec4(loc, 1.0f);
  texCoords = uv;
}