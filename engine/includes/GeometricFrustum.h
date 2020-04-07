//
// Created by Diogo Ribeiro on 02/04/2020.
//

#ifndef GENERATOR_GEOMETRICFRUSTUM_H
#define GENERATOR_GEOMETRICFRUSTUM_H

#ifndef _VEC3_
#include "Vec3.h"
#endif

class Vec3;

#ifndef _PLANE_
#include "FrustumPlane.h"
#endif

class FrustumPlane;

#ifndef _AABOX_
#include "AxisAlignBoxes.h"
#endif

class AABox;



class GeometricFrustum {
private:

    enum {
        TOP = 0,
        BOTTOM,
        LEFT,
        RIGHT,
        NEARP,
        FARP
    };


public:

    enum {OUTSIDE, INTERSECT, INSIDE};

    FrustumPlane pl[6];


    Vec3 ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr;
    float nearD, farD, ratio, angle,tang;
    float nw,nh,fw,fh;

    GeometricFrustum();
    ~GeometricFrustum();

    void setCamInternals(float angle, float ratio, float nearD, float farD);
    void setCamDef(Vec3 &p, Vec3 &l, Vec3 &u);
    int pointInFrustum(Vec3 &p);
    int triangleInFrustum(Vec3 &p1,Vec3 &p2,Vec3 &p3);
    int sphereInFrustum(Vec3 &p, float raio);
    int boxInFrustum(AxisAlignBoxes &b);

    void drawPoints();
    void drawLines();
    void drawPlanes();
    void drawNormals();
};

#endif //GENERATOR_GEOMETRICFRUSTUM_H
