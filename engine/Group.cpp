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
    tracing.resize(MAX_TRACE);
}

void Group::popDraw(int idx, GLuint * buffers, GLuint * indexes) {

    Object3d obj = drawings[idx].getObj();
    glColor3f(drawings[idx].getRed(),drawings[idx].getGreen(),drawings[idx].getBlue());
    unsigned int i = drawings[idx].getBufferId();
    glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes[i]);
    glDrawElements(GL_TRIANGLES, obj.getIndices().size(), GL_UNSIGNED_INT, nullptr);

    // Debug normals.

    glColor3f(0.0f, 0.0f, 0.0f);
    int t;
    vector<GLfloat> normals = obj.getNormals();
    vector<GLfloat> points = obj.getPontos();

    for(int ix = 0; ix < obj.getNumVertices(); ix++) {
        t = 3*ix;

        glBegin(GL_LINES);
        glVertex3f(points[t] ,points[t+1] , points[t+2] );           // Pi
            glVertex3f(points[t] + normals[t] ,
                    points[t+1] + normals[t+1],
                    points[t+2] + normals[t+2]);           // Pi + Ni
        glEnd();
    }
}

void Group::pushTrace(float *mat) {
    Vec3 vec = (*new Vec3(mat[12], mat[13], mat[14]));
    if(N < MAX_TRACE) {
        tracing[N++] = vec;
    }
    else {
        tracing[init] = vec;
        init = (init + 1) % MAX_TRACE;
    }
}

void Group::drawTracing() {
    glBegin(GL_LINES);

    for(int i = 0; i < N; i++) {
        Vec3 tmp = tracing[(init + i) % MAX_TRACE];
        glVertex3f(tmp.getX(), tmp.getY(), tmp.getZ());
    }

    glEnd();
}

void Group::pushTransformation(TransformEvent te) {
    transformations.push_back(te);
}

void Group::pushDraw(DrawEvent de) {
    drawings.push_back(de);
}

int Group::publish(GLuint * buffers, GLuint * indexes, int milis, float *viewMatrix) {
    float tmp[16];

    for (auto & transformation : transformations) {
        transformation.process(milis);
    }

    glGetFloatv(GL_MODELVIEW_MATRIX, tmp);
    Group::pushTrace(tmp);

    glPushMatrix();
    glLoadMatrixf(viewMatrix);
    glMultMatrixf(tmp);

    for (int j = 0; j < drawings.size(); ++j) {
        popDraw(j, buffers, indexes);
    }

    glPopMatrix();

    return n_subgroups;
}

int Group::addSubgroup() {
    return ++n_subgroups;
}

vector<int> Group::getUpgroup()
{
    return n_upGroups;
}
void Group::setUpgroup(int upGroupIndex, vector<int> upGroupParents) {
    this->n_upGroups = upGroupParents;
    this->n_upGroups.push_back(upGroupIndex);
}

void Group::adjustCenter(vector<Group*> groups,int milis) {
    float matrixf [16];
    glPushMatrix();
    glLoadIdentity();
    for (int ui : this->getUpgroup())
    {
        for (auto & transformation : groups[ui]->transformations) {
            transformation.process(milis);
        }
    }
    for (auto & transformation : this->transformations) {
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

