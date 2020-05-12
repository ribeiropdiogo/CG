//
// Created by syrayse on 10/03/20.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <EngineMotion.h>

#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <Shader.h>
#include <mathAct.h>

#endif
#include "Group.h"
#include "IL/il.h"

EngineMotion Group::motion;

Group::Group() {
    n_subgroups = 0;
    center[0]=center[1]=center[2]=0.0;
    center[3]=1.0;
    tracing.resize(MAX_TRACE);
}

void Group::popDraw(int idx, GLuint * vaos, GLuint * textures, GLuint * materials, GLuint lights, vector<Shader> progs) {
    Object3d obj = drawings[idx].getObj();
    unsigned int id = drawings[idx].getBufferId();

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, materials[id]);
    //glBindBuffer(GL_UNIFORM_BUFFER, materials[id]);
    glBindTexture(GL_TEXTURE_2D, textures[id]);
    glBindVertexArray(vaos[id]);

    glDrawElements(GL_TRIANGLES, obj.getIndices().size(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

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

int Group::publish(GLuint * vaos, GLuint * textures, GLuint * materials, GLuint lights, vector<Shader> progs, int milis) {
    GLuint blockIndex;

    for (auto & transformation : transformations) {
        transformation.process(milis);
    }

    for(int i = 0; i < progs.size(); i++) {
        Shader tmp = progs[i];
        tmp.use();
        mt::bindModelViewMatrix(tmp.getID());
        mt::bindNormalMatrix(tmp.getID());
        mt::bindViewPos(progs[0].getID());
        mt::bindTrans(tmp.getID());
        glUniform1i(glGetUniformLocation(tmp.getID(), "ourTexture"), 0);
        blockIndex = glGetUniformBlockIndex(tmp.getID(), "Materials");
        glUniformBlockBinding(tmp.getID(), blockIndex, 1);
        //glBindBufferBase(GL_UNIFORM_BUFFER, 0, lights);
        //cout << blockIndex << endl;
    }
    //mt::bindLight(progs[1].getID());

    for (int j = 0; j < drawings.size(); ++j) {
        popDraw(j, vaos, textures, materials, lights, progs);
    }

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
/*
void Group::adjustCenter(vector<Group*> groups,int milis) {
    float matrixf [16];
    mt::pushMatrix();//glPushMatrix();
    mt::identity();//glLoadIdentity();
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
}*/

float Group::getCenterX() {
    return center[0];
}
float Group::getCenterY() {
    return center[1];
}
float Group::getCenterZ() {
    return center[2];
}

