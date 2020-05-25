#include <iostream>
#include "lightSystem.h"

#define MAX_LIGHT_UNITS 20

#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2

int usedLights = 0;
GLuint lights;

typedef struct light {
    // Color components
    glm::vec4 diffuse;
    glm::vec4 ambient;
    glm::vec4 specular;

    // extrinsic light props
    glm::vec4 position;
    glm::vec4 direction;
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
    Light* light = lighting + usedLights;

    light->ambient = glm::vec4(ambient, 1.0);
    light->diffuse = glm::vec4(diffuse, 1.0);
    light->specular = glm::vec4(specular, 1.0);
}

void setupAttenuation(Light * light, float att_constant, float att_linear, float att_quadratic) {
    light->att_constant = att_constant;
    light->att_linear = att_linear;
    light->att_quadratic = att_quadratic;
}

void lightSystem::addPointLight(glm::vec3 position, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular
        , float att_constant, float att_linear, float att_quadratic) {
    Light* light = lighting + usedLights;

    light->isOn = 1;

    light->type = POINT_LIGHT;
    setupColorComponents(diffuse, ambient, specular);

    light->position = glm::vec4(position, 1.0f);

    setupAttenuation(light, att_constant, att_linear, att_quadratic);
    usedLights++;
}

void lightSystem::addDirectionalLight(glm::vec3 direction, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular
        , float att_constant, float att_linear, float att_quadratic) {
    Light* light = lighting + usedLights;

    light->isOn = 1;
    light->type = DIRECTIONAL_LIGHT;
    setupColorComponents(diffuse, ambient, specular);

    light->direction = glm::vec4(direction, 1.0f);

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

    light->position = glm::vec4(position, 1.0f);
    light->direction = glm::vec4(direction, 1.0f);

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

void lightSystem::activate(GLuint id_shader) {
    GLuint blockIndex = glGetUniformBlockIndex(id_shader, "Lights");
    glUniformBlockBinding(id_shader, blockIndex, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, lights);
}

void lightSystem::setup() {
    // Initially all lights are turned off.
    for(int i = 0; i < MAX_LIGHT_UNITS; i++) {
        lighting[i].isOn = 0;
    }
}