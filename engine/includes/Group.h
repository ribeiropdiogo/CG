//
// Created by syrayse on 10/03/20.
//

#ifndef GENERATOR_GROUP_H
#define GENERATOR_GROUP_H

#include <vector>
#include "TransformEvent.h"
#include "EngineMotion.h"
#include "Shader.h"
#include "Object3d.h"
#include <glm/glm.hpp>

using namespace std;

class Group {
private:
    vector<TransformEvent> transformations;
    vector<Object3d> drawings;
    GLfloat center [4];
    int n_subgroups;
    vector<int> n_upGroups;
public:
    Group();
    void pushTransformation(TransformEvent te);
    void pushDraw(Object3d de);

    int publish(Shader shader, int milis);
    int addSubgroup();
    vector<int> getUpgroup();
    void setUpgroup(int upGroupIndex,vector<int> upGroupParents);
    void adjustCenter(vector<Group*> groups,int milis);
    float getCenterX();
    float getCenterY();
    float getCenterZ();
};

#endif //GENERATOR_GROUP_H
