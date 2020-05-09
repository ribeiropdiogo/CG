//
// Created by syrayse on 09/05/20.
//

#include "mathAct.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace std;

int N = 0;
vector<glm::mat4> stack = {glm::mat4(1.0f)};
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 proj = glm::mat4(1.0f);

void mt::pushMatrix() {
    N++;
    stack.push_back(stack.back());
}

void mt::popMatrix() {
    N--;
    stack.pop_back();
}

void mt::translate(glm::vec3 v) {
    stack[N] = glm::translate(stack.back(), v);
}

void mt::scale(glm::vec3 s) {
    stack[N] = glm::translate(stack.back(), s);
}

void mt::rotate(GLfloat angle, glm::vec3 v) {
    stack[N] = glm::rotate(stack.back(), angle, v);
}

void mt::lookat(glm::vec3 eye, glm::vec3 ls, glm::vec3 upv) {
    view = glm::lookAt(eye, ls, upv);
}

void mt::identity() {
    stack[N] = glm::mat4(1.0f);
}

void mt::loadProj(float * p) {
    proj = glm::make_mat4(p);
}

void mt::bindProj(GLuint id) {
    glUniformMatrix4fv(glGetUniformLocation(id,"projection"),1,GL_FALSE,glm::value_ptr(proj));
}

void mt::bindView(GLuint id) {
    glUniformMatrix4fv(glGetUniformLocation(id,"view"),1,GL_FALSE,glm::value_ptr(view));
}

void mt::bindTrans(GLuint id) {
    glUniformMatrix4fv(glGetUniformLocation(id,"model"),1,GL_FALSE,glm::value_ptr(stack[N]));
}