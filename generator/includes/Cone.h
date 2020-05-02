//
// Created by angelo on 28/02/20.
//

#ifndef GENERATOR_CONE_H
#define GENERATOR_CONE_H

#include <Vec3.h>
#include "Figure.h"

class Cone : public Figure {
public:
    Cone(float radius, float height, int stacks, int slices);
private:
    void polarVertex(float al, float radius, float sz, int j, float ratio);
Vec3 getGradient(float angle, float c);
};

#endif //GENERATOR_CONE_H
