//
// Created by syrayse on 02/03/20.
//

#ifndef GENERATOR_SPHERE_H
#define GENERATOR_SPHERE_H

#include "Figure.h"

class Sphere : private Figure {
public:
    Sphere(float radius, int stacks, int slices);
};

#endif //GENERATOR_SPHERE_H