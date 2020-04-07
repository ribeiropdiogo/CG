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
    int n_subgroups;
    Vec3 a;
    Vec3 b;
    Vec3 c;
    void popDraw(int idx, GLuint * buffers, GLuint * indexes);
public:
    Group();
    void pushTransformation(TransformEvent te);
    void pushDraw(DrawEvent de);
    int publish(GLuint * buffers, GLuint * indexes, int milis);
    int addSubgroup();
};

#endif //GENERATOR_GROUP_H
