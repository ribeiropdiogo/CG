#version 330 core

#define MAX_NUM_LIGHTS 20

out vec4 FragColor;

struct Light {
	bool isOn;
	int type;

	// Color components
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;

	// extrinsic light props
	vec4 position;
	vec4 direction;
	float emissionAngle;

	// caracteristics of intensity
	float atenuation;
};

layout (std140) uniform Materials {
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    float shininess;
};

layout (std140) uniform Lights {
	Light lights[MAX_NUM_LIGHTS];
};
 

in vec2 texCoord;

uniform sampler2D ourTexture;
 
void main() {
 	vec4 texColor = texture(ourTexture, texCoord);

    FragColor = (diffuse + ambient);//(ambient + diffuse + specular) ;//* texColor;
}