//
// Created by syrayse on 10/03/20.
//

#ifndef GENERATOR_GROUP_H
#define GENERATOR_GROUP_H

#include <vector>
#include "TransformEvent.h"
#include "DrawEvent.h"
#include "EngineMotion.h"

using namespace std;

class Group {
private:
    static EngineMotion motion;
    vector<TransformEvent> transformations;
    vector<DrawEvent> drawings;
    GLfloat center [4];
    int n_subgroups;
    vector<int> n_upGroups;
    vector<Vec3> tracing;
    int init=0, N=0,MAX_TRACE=200;

    void popDraw(int idx, GLuint * vaos, GLuint * textures, unsigned int progs);
public:
    Group();
    void pushTransformation(TransformEvent te);
    void pushDraw(DrawEvent de);
    void pushTrace(float *mat);

    int publish(GLuint * vaos, GLuint * textures, unsigned int progs, int milis);
    void drawTracing();
    int addSubgroup();
    vector<int> getUpgroup();
    void setUpgroup(int upGroupIndex,vector<int> upGroupParents);
    void adjustCenter(vector<Group*> groups,int milis);
    float getCenterX();
    float getCenterY();
    float getCenterZ();
};

#endif //GENERATOR_GROUP_H
