//
// Created by syrayse on 10/03/20.
//

#ifndef GENERATOR_GROUP_H
#define GENERATOR_GROUP_H

#include <vector>
#include "TransformEvent.h"
#include "DrawEvent.h"

using namespace std;

class Group {
private:
    vector<TransformEvent> transformations;
    vector<DrawEvent> drawings;
    int n_subgroups;
    void popDraw(int idx, GLuint * buffers, GLuint * indexes);
public:
    Group();
    void pushTransformation(TransformEvent te);
    void pushDraw(DrawEvent de);
    int publish(GLuint * buffers, GLuint * indexes);
    int addSubgroup();
};

#endif //GENERATOR_GROUP_H
