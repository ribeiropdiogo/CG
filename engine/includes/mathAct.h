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
    void lookat(glm::vec3 eye, glm::vec3 ls, glm::vec3 upv);
    void identity();
    void loadProj(float* p);
    void bindProj(GLuint id);
    void bindView(GLuint id);
    void bindTrans(GLuint id);
}

#endif //GENERATOR_MATHACT_H
