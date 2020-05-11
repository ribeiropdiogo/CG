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
	int isOn;
	int type;

	// Color components
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;

	// extrinsic light props
	vec3 position;
	vec3 direction;
	float emissionAngle;

};

struct Material {
	vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    float shininess;
};

layout (std140) uniform Materials {
    Material mat;
};

layout (std140) uniform Lights {
	Light lights[MAX_LIGHT_UNITS];
};

uniform vec3 viewPos;

in vec3 fragPos;
in vec2 texCoord;
in vec3 normCoord;

uniform sampler2D ourTexture;
uniform mat3 normalMatrix;

vec3 calcDirLight(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    
    float diff = max(dot(normal, lightDir), 0.0);
    
    // half vector from Brinn.
    vec3 halfvector = normalize(lightDir + viewDir);

    float spec = pow(max(dot(halfvector, normal), 0.0), mat.shininess);
    
    vec3 ambient  = light.ambient ;
    vec3 diffuse  =  light.diffuse  * diff;
    vec3 specular =  light.specular * spec ;

    return (ambient + diffuse + specular);
}  
 
vec3 calcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
    
	// half vector from Brinn.
    vec3 halfvector = normalize(lightDir + viewDir);

    float spec = pow(max(dot(halfvector, normal), 0.0), mat.shininess);

    // attenuation
    float distance    = length(light.position - fragPos);
    //float attenuation = 1.0 / ((distance * distance));   

    // combine results
    vec3 ambient  = light.ambient  ;
    vec3 diffuse  =  light.diffuse  * diff ;
    vec3 specular =  light.specular * spec ;
    /*ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;*/
    return (ambient + diffuse + specular);
} 

vec3 calcSpotLight(Light light, vec3 fragPos, vec3 normal, vec3 viewDir) {
	vec3 color = vec3(0.0);
	vec3 lightDir = normalize(light.position - fragPos);
	float attenuation, spotEffect, dist = length(lightDir);
    vec3 halfvector = normalize(lightDir + viewDir);
    vec3 ambient, diffuse, specular;
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(halfvector, normal), 0.0), mat.shininess);

	if( diff > 0.0 ) {

		spotEffect = dot(normalize(light.direction),normalize(-lightDir));

		if(spotEffect > light.emissionAngle) {
			attenuation = spotEffect / (dist * dist);
			ambient  = light.ambient  ;
    		diffuse  =  light.diffuse  * diff ;
    		specular =  light.specular * spec ;
    		color += attenuation * (ambient + diffuse + specular);
		}

	}

	return color;
}

void main() {
	vec3 Normal =  normalMatrix * normCoord;

	vec3 viewDir = normalize(viewPos - fragPos);

 	vec4 texColor = texture(ourTexture, texCoord);
 	vec3 res = vec3(0.0);

 	for(int i = 0; i < MAX_LIGHT_UNITS; i++) {

 		if(lights[i].isOn == 1) {

 			switch(lights[i].type){
 				case POINT_LIGHT:
 					res += calcPointLight(lights[i],Normal,fragPos,viewDir);
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


 	FragColor = vec4(res,1.0);//(ambient + diffuse + specular) * texColor;
}