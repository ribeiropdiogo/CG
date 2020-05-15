#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormCoord;
layout (location = 2) in vec2 aTexCoord;

out vec3 fragPos;
out vec2 texCoord;
out vec3 normCoord;

uniform mat4 modelViewProj;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{ 
    gl_Position = modelViewProj * vec4(aPos,1.0);

    texCoord = aTexCoord;
    normCoord = normalize(normalMatrix * aNormCoord);
    fragPos = vec3( model * vec4(aPos,1));
} 