//
// Created by Diogo Ribeiro on 21/02/2020.
//

#ifndef GENERATOR_BOX_H
#define GENERATOR_BOX_H


#include "Figure.h"

class Box : private Figure {
public:
    Box(float wx, float wy, float wz);
    Box(float wx, float wy, float wz,
            int divisions);
};

#endif //GENERATOR_BOX_H