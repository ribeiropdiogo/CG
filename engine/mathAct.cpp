//
// Created by syrayse on 09/05/20.
//

#include "mathAct.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

using namespace std;

int N = 0;
vector<glm::mat4> stack = {glm::mat4(1.0f)};
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 proj = glm::mat4(1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f);

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
    stack[N] = glm::scale(stack.back(), s);
}

void mt::rotate(GLfloat angle, glm::vec3 v) {
    stack[N] = glm::rotate(stack.back(), angle, v);
}

void mt::lookat(glm::vec3 eye, glm::vec3 ls, glm::vec3 upv) {
    cameraPos = eye;
    view = glm::lookAt(eye, ls, upv);
}

void mt::multMatix(glm::mat4 mt) {
    stack[N] = stack[N] * mt;
}

void mt::identity() {
    stack[N] = glm::mat4(1.0f);
}

void mt::perspective(GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far) {
    proj = glm::perspective(fovy, aspect, near, far);
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

void mt::bindNormalMatrix(GLuint id) {
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(stack[N])));
    glUniformMatrix3fv(glGetUniformLocation(id,"normalMatrix"),1,GL_FALSE,glm::value_ptr(normalMatrix));
}

void mt::bindModelViewMatrix(GLuint id) {
    glm::mat4 mvp = proj * view * stack[N];
    glUniformMatrix4fv(glGetUniformLocation(id,"modelViewProj"),1,GL_FALSE,glm::value_ptr(mvp));
}

void mt::bindViewPos(GLuint id) {
    glUniform3f(glGetUniformLocation(id,"viewPos"),cameraPos.x,cameraPos.y,cameraPos.z);
}

void mt::undoViewTranslation() {
    view = glm::mat4(glm::mat3(view));
}

glm::vec4 mt::getModelViewPos() {
    glm::mat4 mv = view * stack[N];
    return glm::vec4(mv[3][0], mv[3][1], mv[3][2], mv[3][3]);
}