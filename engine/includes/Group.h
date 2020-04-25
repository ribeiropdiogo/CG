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
    void popDraw(int idx, GLuint * buffers, GLuint * indexes);
public:
    Group();
    void pushTransformation(TransformEvent te);
    void pushDraw(DrawEvent de);

    int publish(GLuint * buffers, GLuint * indexes, int milis);
    int addSubgroup();
    vector<int> getUpgroup();
    void setUpgroup(int upGroupIndex,vector<int> upGroupParents);
    void adjustCenter(vector<Group*> groups,int milis);
    float getCenterX();
    float getCenterY();
    float getCenterZ();
};

#endif //GENERATOR_GROUP_H
