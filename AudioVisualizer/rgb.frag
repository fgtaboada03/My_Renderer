#version 330 core

// Outputs Color in RGBA
out vec4 FragColor;

// Inputs
in vec3 colors;

void main()
{
	FragColor = vec4(colors, 1.0);
}