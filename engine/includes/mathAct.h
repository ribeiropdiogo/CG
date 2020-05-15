//
// Created by syrayse on 09/05/20.
//

#ifndef GENERATOR_MATHACT_H
#define GENERATOR_MATHACT_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>

namespace mt {
    void pushMatrix();
    void popMatrix();
    void translate(glm::vec3 v);
    void scale(glm::vec3 s);
    void rotate(GLfloat angle, glm::vec3 v);
    void perspective(GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far);
    void lookat(glm::vec3 eye, glm::vec3 ls, glm::vec3 upv);
    void multMatix(glm::mat4 mt);
    void identity();
    void bindProj(GLuint id);
    void bindView(GLuint id);
    void bindTrans(GLuint id);
    void bindLight(GLuint id);
    void bindNormalMatrix(GLuint id);
    void bindModelViewMatrix(GLuint id);
    void bindViewPos(GLuint id);
    void undoViewTranslation();
    glm::vec4 getModelViewPos();
}

#endif //GENERATOR_MATHACT_H
