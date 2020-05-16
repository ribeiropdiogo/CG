//
// Created by syrayse on 16/05/20.
//

#include "lightSystem.h"

#define MAX_LIGHT_UNITS 20

#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2

int usedLights = 0;
GLuint lights;

typedef struct light {
    // Color components
    float diffuse[4];
    float ambient[4] ;
    float specular[4];

    // extrinsic light props
    float position[4] = {0.0, 0.0, 0.0};
    float direction[4] = {0.0, 0.0, 0.0,0.0};
    int isOn;
    int type;

    float emissionAngle = M_PI_2;

    // Attenuation of light
    float att_constant;
    float att_linear;
    float att_quadratic;

    float outerCutOff;
    // Just to conform to layout std140.
    float trash;
} Light;

Light lighting[MAX_LIGHT_UNITS];

void setupColorComponents(glm::vec3 diffuse,
                          glm::vec3 ambient, glm::vec3 specular) {
    int i;
    Light* light = lighting + usedLights;
    for(i = 0; i < 3; i++) {
        light->diffuse[i] = diffuse[i];
        light->ambient[i] = ambient[i];
        light->specular[i] = specular[i];
    }
    light->diffuse[i] = 1;
    light->ambient[i] = 1;
    light->specular[i] = 1;
}

void setupAttenuation(Light * light, float att_constant, float att_linear, float att_quadratic) {
    light->att_constant = att_constant;
    light->att_linear = att_linear;
    light->att_quadratic = att_quadratic;
}

void lightSystem::addPointLight(glm::vec3 position, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular
        , float att_constant, float att_linear, float att_quadratic) {
    Light* light = lighting + usedLights;
    //lighting[usedLights].isOn = 1;
    light->isOn = 1;
    //cout << light.isOn << endl;
    light->type = POINT_LIGHT;
    setupColorComponents(diffuse, ambient, specular);
    for(int i = 0; i < 3; i++) {
        light->position[i] = position[i];
    }
    light->position[3] = 1;
    setupAttenuation(light, att_constant, att_linear, att_quadratic);
    usedLights++;
}

void lightSystem::addDirectionalLight(glm::vec3 direction, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular
        , float att_constant, float att_linear, float att_quadratic) {
    Light* light = lighting + usedLights;

    light->isOn = 1;
    light->type = DIRECTIONAL_LIGHT;
    setupColorComponents(diffuse, ambient, specular);
    for(int i = 0; i < 3; i++) {
        light->direction[i] = direction[i];
    }
    light->direction[3] = 1;
    setupAttenuation(light, att_constant, att_linear, att_quadratic);
    usedLights++;
}

void lightSystem::addSpotLight(glm::vec3 position, glm::vec3 direction,
                          glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular, float emissionAngle
        , float outerCutOff, float att_constant, float att_linear, float att_quadratic) {
    Light* light = lighting + usedLights;

    light->isOn = 1;
    light->type = SPOT_LIGHT;
    setupColorComponents(diffuse, ambient, specular);
    for(int i = 0; i < 3; i++) {
        light->position[i] = position[i];
        light->direction[i] = direction[i];
    }
    light->position[3] = 1;
    light->direction[3] = 1;
    light->emissionAngle = emissionAngle;
    light->outerCutOff = outerCutOff;
    setupAttenuation(light, att_constant, att_linear, att_quadratic);
    usedLights++;
}

void lightSystem::bind() {
    glGenBuffers(1, &lights);

    glBindBuffer(GL_UNIFORM_BUFFER, lights);

    glBufferData(GL_UNIFORM_BUFFER, MAX_LIGHT_UNITS*sizeof(Light), lighting, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void lightSystem::setup() {
    // Initially all lights are turned off.
    for(int i = 0; i < MAX_LIGHT_UNITS; i++) {
        lighting[i].isOn = 0;
    }
}

GLuint lightSystem::getID() {
    return lights;
}