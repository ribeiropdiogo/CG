//
// Created by Diogo Ribeiro on 21/02/2020.
//

#ifndef GENERATOR_BOX_H
#define GENERATOR_BOX_H


#include "Figure.h"

class Box : public Figure {
public:
    Box(float wx, float wy, float wz);
    Box(float wx, float wy, float wz,
            int divisions);
private:
    int genStripes(float * currVertex, int divisions, float incX, float incY, float incZ);
    void indexStripes(int divisions, int count);
    void genLateral(float* currVertex, int divisions, float wy, float incY, float incZ);
    void indexLateralBack(int divisions);
    void indexLateralFront(int divisions, int count);
    void indexGap(int sp, int divisions, bool right);
    void indexGapSpecial();
    void bindBand(int* band1, int* band2, int sz);
};

#endif //GENERATOR_BOX_H