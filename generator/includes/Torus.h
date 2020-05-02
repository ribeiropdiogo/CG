//
// Created by Diogo Ribeiro on 07/04/2020.
//

#ifndef GENERATOR_TORUS_H
#define GENERATOR_TORUS_H

#include "Figure.h"
#include <Vec3.h>

class Torus : public Figure{
public:
    Torus(float R, float r, int stacks, int slices);
private:
    Vec3 getGradient(float u, float v, float r, float R);

};


#endif //GENERATOR_TORUS_H
