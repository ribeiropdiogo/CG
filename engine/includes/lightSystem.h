//
// Created by syrayse on 16/05/20.
//

#ifndef GENERATOR_LIGHTSYSTEM_H
#define GENERATOR_LIGHTSYSTEM_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class lightSystem {
private:
public:
    static void addPointLight(glm::vec3 position, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular
            , float att_constant, float att_linear, float att_quadratic);
    static void addDirectionalLight(glm::vec3 direction, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular
            , float att_constant, float att_linear, float att_quadratic);
    static void addSpotLight(glm::vec3 position, glm::vec3 direction,
                      glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular, float emissionAngle
            , float outerCutOff, float att_constant, float att_linear, float att_quadratic);
    static void bind();
    static void setup();
    static GLuint getID();
};

#endif //GENERATOR_LIGHTSYSTEM_H
