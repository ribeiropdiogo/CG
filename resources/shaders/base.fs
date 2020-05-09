#version 330 core

out vec4 FragColor;

in vec2 normCoord;


void main() 
{
	FragColor = vec4(normCoord, 0.0, 1.0);
}