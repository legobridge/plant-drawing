#version 400 core

uniform vec3 myColor;

out vec4 FragColor;

void main()
{
	FragColor = vec4(myColor, 1.0f);
}