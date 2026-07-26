#version 330 core

// Outputs Color in RGBA
out vec4 FragColor;

// Inputs
in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, texCoord);
};