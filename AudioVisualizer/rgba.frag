#version 330 core

// Outputs Color in RGBA
out vec4 FragColor;

// Inputs
in vec4 colors;

void main()
{
	FragColor = colors;
}