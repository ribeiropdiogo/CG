//
// Created by Diogo Ribeiro on 02/04/2020.
//

#ifndef GENERATOR_AXISALIGNBOXES_H
#define GENERATOR_AXISALIGNBOXES_H

#ifndef _Vec3_
#include "Vec3.h"
#endif

class Vec3;

class AxisAlignBoxes {
public:
    Vec3 corner;
    float x,y,z;

    AxisAlignBoxes( Vec3 &corner, float x, float y, float z);
    AxisAlignBoxes(void);
    ~AxisAlignBoxes();
    void setBox( Vec3 &corner, float x, float y, float z);
    Vec3 getVertexP(Vec3 &normal);
    Vec3 getVertexN(Vec3 &normal);
};


#endif //GENERATOR_AXISALIGNBOXES_H
