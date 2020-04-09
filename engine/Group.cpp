//
// Created by syrayse on 10/03/20.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <EngineMotion.h>

#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "Group.h"

EngineMotion Group::motion;

Group::Group() {
    n_subgroups = 0;
    center[0]=center[1]=center[2]=0.0;
    center[3]=1.0;
}

void Group::popDraw(int idx, GLuint * buffers, GLuint * indexes) {
    Object3d obj = drawings[idx].getObj();
    unsigned int i = drawings[idx].getBufferId();
    glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes[i]);
    glDrawElements(GL_TRIANGLES, obj.getIndices().size(), GL_UNSIGNED_INT, nullptr);
}

void Group::pushTransformation(TransformEvent te) {
    transformations.push_back(te);
}

void Group::pushDraw(DrawEvent de) {
    drawings.push_back(de);
}

int Group::publish(GLuint * buffers, GLuint * indexes, int milis) {
    for (auto & transformation : transformations) {
        transformation.process(milis);
    }

    for (int j = 0; j < drawings.size(); ++j) {
        popDraw(j, buffers, indexes);
    }

    return n_subgroups;
}

int Group::addSubgroup() {
    return ++n_subgroups;
}

void Group::adjustCenter(int milis) {
    float matrixf [16];
    glPushMatrix();
    glLoadIdentity();
    for (auto & transformation : transformations) {
        transformation.process(milis);
    }
    glGetFloatv(GL_MODELVIEW_MATRIX,matrixf);
    glPopMatrix();
    center[0] = matrixf[12]*center[3];
    center[1] = matrixf[13]*center[3];
    center[2] = matrixf[14]*center[3];
    center[3] = matrixf[15]*center[3];
    if (center[3]>1.0)
    {
        center[0]/=center[3];
        center[1]/=center[3];
        center[2]/=center[3];
        center[3]/=center[3];
    }
}

float Group::getCenterX() {
    return center[0];
}
float Group::getCenterY() {
    return center[1];
}
float Group::getCenterZ() {
    return center[2];
}