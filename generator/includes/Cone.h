//
// Created by angelo on 28/02/20.
//

#ifndef GENERATOR_CONE_H
#define GENERATOR_CONE_H

#include "Figure.h"

class Cone : public Figure {
public:
    Cone(float radius, float height, int stacks, int slices);
private:
    void polarVertex(double al, double be, float radius, float sz, int i, int j);
};

#endif //GENERATOR_CONE_H
