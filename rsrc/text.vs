#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uvs;

out vec2 texcoords;

uniform vec2 translation;
uniform mat4 ortho;

void main()
{
    gl_Position = ortho * vec4(position + translation, 0.0f, 1.0f);
	texcoords = uvs;
}