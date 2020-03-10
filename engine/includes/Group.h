//
// Created by syrayse on 10/03/20.
//

#ifndef GENERATOR_GROUP_H
#define GENERATOR_GROUP_H

#include "TransformEvent.h"

using namespace std;

class Group {
private:
    queue<TransformEvent> transformations;
    queue<DrawEvent> drawings;
    int n_subgroups;
    void popDraw(GLuint * buffers, GLuint * indexes);
public:
    void pushTransformation(TransformEvent te);
    void pushDraw(DrawEvent de);
    int publish(GLuint * buffers, GLuint * indexes);
};

#endif //GENERATOR_GROUP_H
