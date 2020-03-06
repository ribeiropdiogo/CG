//
// Created by syrayse on 02/03/20.
//

#ifndef GENERATOR_SPHERE_H
#define GENERATOR_SPHERE_H

#include "Figure.h"

class Sphere : public Figure {
public:
    Sphere(float radius, int stacks, int slices);
private:
    void polarVertex(double al, double be, float radius, int i, int j);
};

#endif //GENERATOR_SPHERE_H