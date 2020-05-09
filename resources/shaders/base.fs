#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 normCoord;

uniform sampler2D ourTexture;

void main() 
{
	FragColor =  texture(ourTexture, texCoord);
}