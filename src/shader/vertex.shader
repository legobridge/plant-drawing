#version 400 core

layout (location = 0) in vec3 pos;

uniform vec2 dimensions;
uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(pos, 1.0f);
}