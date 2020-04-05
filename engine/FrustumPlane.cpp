//
// Created by Diogo Ribeiro on 02/04/2020.
//

#include "FrustumPlane.h"
#include <stdio.h>


FrustumPlane::FrustumPlane( Vec3 &v1,  Vec3 &v2,  Vec3 &v3) {

    set3Points(v1,v2,v3);
}


FrustumPlane::FrustumPlane() {}

FrustumPlane::~FrustumPlane() {}


void FrustumPlane::set3Points( Vec3 &v1,  Vec3 &v2,  Vec3 &v3) {


    Vec3 aux1, aux2;

    aux1 = v1 - v2;
    aux2 = v3 - v2;

    normal = aux2.crossprod(aux1);

    normal.normalize();
    point = (v2);
    d = -(normal.dotprod(point));
}

void FrustumPlane::setNormalAndPoint(Vec3 &normal, Vec3 &point) {

    this->normal = (normal);
    this->normal.normalize();
    d = -(this->normal.dotprod(point));
}

void FrustumPlane::setCoefficients(float a, float b, float c, float d) {

    // set the normal vector
    normal = *new Vec3(a, b, c);
    //compute the lenght of the vector
    float l = normal.length();
    // normalize the vector
    normal.normalize();
    // and divide d by th length as well
    this->d = d/l;
}




float FrustumPlane::distance(Vec3 p) {

    return (d + normal.dotprod(p));
}

void FrustumPlane::print() {

    printf("Plane(");
    //normal.print();
    printf("# %f)",d);
}

