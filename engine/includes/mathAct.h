/**
 * Defintion of the math system utilized throughtout the system.
 *
 * Allowing for more control over the arguments passed to the shader.
 */
#ifndef GENERATOR_MATHACT_H
#define GENERATOR_MATHACT_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>

namespace mt {
    /**
     * Pushes the current matrix onto the matrix stack.
     */
    void pushMatrix();

    /**
     * Pops the previously pushed matrix from the stack,
     * setting it as the de facto system matrix in use.
     */
    void popMatrix();

    /**
     * Performs a vector translation.
     * @param v Translation used.
     */
    void translate(glm::vec3 v);

    /**
     * Performs a scale transformation.
     * @param s Scaling applied.
     */
    void scale(glm::vec3 s);

    /**
     * Applies a matrix rotation in accordance with
     * the provided angle and axis of rotation.
     *
     * @param angle Angle of rotation.
     * @param v Axis of rotation.
     */
    void rotate(GLfloat angle, glm::vec3 v);

    /**
     * Sets up the projection matrix.
     *
     * @param fovy  Rate used.
     * @param aspect Aspect used in the projection matrix.
     * @param near Near of the engine.
     * @param far Far of the engine.
     */
    void perspective(GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far);

    /**
     * Sets up the camera in the space.
     *
     * @param eye Eye position of the camera.
     * @param ls Line of sight of the camera.
     * @param upv Up vector of the camera
     */
    void lookat(glm::vec3 eye, glm::vec3 ls, glm::vec3 upv);

    /**
     * Multiplies the current system matrix by the
     * provided matrix.
     *
     * @param mt Matrix used in the multiplication.
     */
    void multMatix(glm::mat4 mt);

    /**
     * Sets the system matrix as an identity matrix.
     */
    void identity();

    /**
     * Binds the system projection matrix with the
     * provided shader id.
     *
     * @param id Shader's ID.
     */
    void bindProj(GLuint id);

    /**
     * Binds the system view matrix with the provided
     * shader id
     *
     * @param id Shader's ID
     */
    void bindView(GLuint id);

    /**
     * Binds the system transformation matrix with
     * the provided shader id.
     *
     * @param id Shader's ID.
     */
    void bindTrans(GLuint id);

    /**
     * Binds the system's normal matrix with
     * the provided shader id.
     *
     * @param id Shader's ID.
     */
    void bindNormalMatrix(GLuint id);

    /**
     * Binds the system's model view matriz
     * with the provided shader id.
     *
     * @param id Shader's ID.
     */
    void bindModelViewMatrix(GLuint id);

    /**
     * Binds the view position of the system
     * with the provided shader id.
     *
     * @param id Shader's ID.
     */
    void bindViewPos(GLuint id);

    /**
     * Undos all of the translations
     * applied to the view camera.
     */
    void undoViewTranslation();

    /**
     * Indicates the current system's position
     * in the system.
     *
     * @return THe current system position.
     */
    glm::vec4 getModelPos();
}

#endif
