//
// Created by Diogo Ribeiro on 02/04/2020.
//

#ifndef GENERATOR_FRUSTUMPLANE_H
#define GENERATOR_FRUSTUMPLANE_H

#ifndef _Vec3_
#include "Vec3.h"
#endif

class Vec3;


class FrustumPlane {
public:

    Vec3 normal,point;
    float d;


    FrustumPlane( Vec3 &v1,  Vec3 &v2,  Vec3 &v3);
    FrustumPlane(void);
    ~FrustumPlane();

    void set3Points( Vec3 &v1,  Vec3 &v2,  Vec3 &v3);
    void setNormalAndPoint(Vec3 &normal, Vec3 &point);
    void setCoefficients(float a, float b, float c, float d);
    float distance(Vec3 p);

    void print();

};


#endif //GENERATOR_FRUSTUMPLANE_H
