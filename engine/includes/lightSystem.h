/**
 * API necessary for the managment of the light system.
 */
#ifndef GENERATOR_LIGHTSYSTEM_H
#define GENERATOR_LIGHTSYSTEM_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class lightSystem {
public:
    /**
     * Introduces a new point light onto the light system.
     *
     * @param position Position of the point light.
     * @param diffuse Diffuse color component of the light.
     * @param ambient Ambient color component of the light.
     * @param specular Specular color component of the light.
     * @param att_constant Atenuattion constant being in use.
     * @param att_linear Linear attenuation being considered.
     * @param att_quadratic Quadratica attenuation being considered.
     */
    static void
    addPointLight(glm::vec3 position, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular, float att_constant,
                  float att_linear, float att_quadratic);

    /**
     * Introduces a new directional light onto the light system.
     *
     * @param direction Direction of the directional light.
     * @param diffuse Diffuse color component of the light.
     * @param ambient Ambient color component of the light.
     * @param specular Specular color component of the light.
     * @param att_constant Atenuattion constant being in use.
     * @param att_linear Linear attenuation being considered.
     * @param att_quadratic Quadratica attenuation being considered.
     */
    static void addDirectionalLight(glm::vec3 direction, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular,
                                    float att_constant, float att_linear, float att_quadratic);

    /**
     * Introduces a new directional light onto the light system.
     *
     * @param position Position of the spot light.
     * @param direction Direction of the spot light.
     * @param diffuse Diffuse color component of the light.
     * @param ambient Ambient color component of the light.
     * @param specular Specular color component of the light.
     * @param emissionAngle Minor angle of emission.
     * @param outerCutOff Outer most angle of emission.
     * @param att_constant Atenuattion constant being in use.
     * @param att_linear Linear attenuation being considered.
     * @param att_quadratic Quadratica attenuation being considered.
     */
    static void addSpotLight(glm::vec3 position, glm::vec3 direction,
                             glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular, float emissionAngle,
                             float outerCutOff, float att_constant, float att_linear, float att_quadratic);

    /**
     * Binds the light system onto the OpenGL mechanisms and necessary shaders.
     */
    static void bind();

    /**
     * Initializes the light system, turning off all of the lights initially.
     */
    static void setup();

    /**
     * Activates the lightning system in the necessary shader,
     * taking care of all of the necessary uniform blocks.
     *
     * @param id_shader ID of the shader being used.
     */
    static void activate(GLuint id_shader);
};

#endif