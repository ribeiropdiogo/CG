#version 330 core

// Any changes to defines should be reflected
// on the engine per si.

// Maximum number of lights supported
#define MAX_LIGHT_UNITS 20

// Different types of lights
#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2

out vec4 FragColor;

struct Light {
	// Color components
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;

	// extrinsic light props
	vec4 position;
	vec4 direction;
    int isOn;
    int type;

    float innerCutOff;
    float att_constant;
    float att_linear;
    float att_quadratic;

    // for spot light;
    float outerCutOff;

    // just to conform to layout std140
    float trash;
};

struct Material {
	vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    float shininess;
};

layout (std140) uniform Materials {
    Material mat;
};

layout (std140) uniform Lights {
	Light lights[MAX_LIGHT_UNITS];
};

uniform vec3 viewPos;
uniform mat4 model;

in vec3 fragPos;
in vec2 texCoord;
in vec3 normCoord;

uniform sampler2D ourTexture;
uniform mat3 normalMatrix;

vec3 calcDirLight(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-vec3(light.direction));
    
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    
    vec4 ambient  = light.ambient * mat.ambient;
    vec4 diffuse  =  light.diffuse  * diff * mat.diffuse;
    vec4 specular =  light.specular * spec * mat.specular;

    return vec3(ambient + diffuse + specular);
}
 
vec3 calcPointLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(vec3(light.position) - fragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    
    float distance    = length(vec3(light.position) - fragPos);
    float attenuation = 1.0 / (light.att_constant + light.att_linear * distance + 
                 light.att_quadratic * (distance * distance));   

    // combine results
    vec4 ambient  = light.ambient * mat.ambient;
    vec4 diffuse  =  light.diffuse  * diff * mat.diffuse;
    vec4 specular =  light.specular * spec * mat.specular;

    return vec3(ambient + diffuse + specular) * attenuation;
} 

vec3 calcSpotLight(Light light, vec3 fragPos, vec3 normal, vec3 viewDir) {
	vec3 color = vec3(0.0);
	vec3 lightDir = normalize(vec3(light.position) - fragPos);
	float attenuation, spotEffect, dist = length(lightDir);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec4 ambient, diffuse, specular;
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    float distance    = length(vec3(light.position) - fragPos);  

    attenuation = 1 / (light.att_constant + light.att_linear * distance +
                 light.att_quadratic * (distance * distance));

    if(diff > 0.0) {
		spotEffect = dot(normalize(vec3(light.direction)),normalize(-lightDir));

        float epsilon   = light.innerCutOff - light.outerCutOff;
        float intensity = clamp((spotEffect - light.outerCutOff) / epsilon, 0.0, 1.0);

		if(spotEffect > light.outerCutOff) {
			ambient  = light.ambient  * mat.ambient;
    		diffuse  =  light.diffuse  * diff * mat.diffuse * intensity;
    		specular =  light.specular * spec * mat.specular * intensity;
    		color += vec3(ambient + diffuse + specular);
		}
        else {
            color = vec3(light.ambient * mat.ambient) ;
        }
    }

	return color * attenuation;
}

void main() {
	vec3 Normal = normCoord;

	vec3 viewDir = normalize(viewPos - vec3(fragPos));

 	vec4 texColor = texture(ourTexture, texCoord);
 	vec3 res = vec3(0.0);

 	for(int i = 0; i < MAX_LIGHT_UNITS; i++) {

 		if(lights[i].isOn == 1) {

 			switch(lights[i].type){
 				case POINT_LIGHT:
 					res += calcPointLight(lights[i],Normal,viewDir);
 					break;
				case DIRECTIONAL_LIGHT:
					res += calcDirLight(lights[i],Normal,viewDir);
					break;
				case SPOT_LIGHT:
 					res += calcSpotLight(lights[i],fragPos,Normal,viewDir);
 					break;
 			}

 		}

 	}


 	FragColor = vec4(res, 1.0) * texColor;//(ambient + diffuse + specular) * texColor;
}