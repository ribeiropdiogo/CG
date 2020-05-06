//
// Created by Diogo Ribeiro on 07/04/2020.
//
#include <cmath>
#include <iostream>
#include <Vec3.h>
#include "Torus.h"

Torus::Torus(float R, float r, int stacks, int slices) : Figure(stacks, slices) {
    int k1, k2;
    float thetainc = 2.0f*M_PI / slices;
    float phiinc = 2*M_PI / stacks;
    float theta = 0.0, phi;

    float x,y,z;


    for (int i = 0; i <= slices; i++){
        phi = 0.0;

        for (int j = 0; j <= stacks; j++){
            x = (R+r*cos(phi))*cos(theta);
            y = r*sin(phi);
            z = (R+r*cos(phi))*sin(theta);
            Torus::addVertice((float)x,(float)y,(float)z);
            Vec3 p = Torus::getGradient(phi,theta,r,R).normalize();
            Figure::addNormal(p.getX(), p.getY(), p.getZ());
            Figure::addTexCoord((float)i / (float)slices,
                                (float)j / (float)stacks);
            phi += phiinc;
        }

        theta += thetainc;
    }

    for(int i = 0; i < slices; i++) {

        k1 = i * (stacks + 1);
        k2 = k1 + stacks + 1;

        for(int j = 0; j < stacks; j++, k1++, k2++) {
            Figure::addIndex(k2 + 1, k1, k1 + 1);
            Figure::addIndex(k2, k1, k2 + 1);
        }

    }
}

Vec3 Torus::getGradient(float u, float v, float r, float R) {
    // Calcular du.
    Vec3 du = * new Vec3(
            -r*cos(v)*sin(u),
            r*cos(u),
            -r*sin(v)*sin(u));

    // Calcular dv.
    Vec3 dv = * new Vec3(
            -R*sin(v) - r*sin(v)*cos(u),
            0.0f,
            R*cos(v) + r*cos(v)*cos(u));

    return du.crossprod(dv);
}
