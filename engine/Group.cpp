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
}

void Group::pushTransformation(TransformEvent te) {
    transformations.push_back(te);
}

void Group::pushDraw(Object3d de) {
    drawings.push_back(de);
}

int Group::publish(GLuint * vaos, GLuint * textures, GLuint * materials, GLuint lights, vector<Shader> progs, int milis) {
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
    }

    for (int j = 0; j < drawings.size(); ++j) {
        drawings[j].draw(progs[0].getID());
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

void Group::adjustCenter(vector<Group*> groups,int milis) {
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
    glm::vec4 mv = mt::getModelPos();
    mt::popMatrix();
    center[0] = mv[0]*center[3];
    center[1] = mv[1]*center[3];
    center[2] = mv[2]*center[3];
    center[3] = mv[3]*center[3];
    if (center[3]>1.0)
    {
        center[0]/=center[3];
        center[1]/=center[3];
        center[2]/=center[3];
        center[3]=1.0;
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

