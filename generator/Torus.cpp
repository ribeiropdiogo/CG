//
// Created by Diogo Ribeiro on 07/04/2020.
//
#include <cmath>
#include <iostream>
#include <Vec3.h>
#include "Torus.h"

Torus::Torus(float R, float r, int stacks, int slices) : Figure(stacks, slices) {
    float thetainc = 2*M_PI / slices;
    float phiinc = 2*M_PI / stacks;
    float theta = 0.0, phi = 0.0;
    int halfstacks = stacks/2;

    float x,y,z;

    //cout<<"sides: "<<slices<<endl;

    for (int i = 0; i < slices; i++){
        phi = 0.0;
        //cout<<"anel "<<i+1<<endl;
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


    int verts = Torus::getVerticeSize()/3;

    cout<<"vertices: "<<verts<<endl;
    int j = 0;
    for (j = 0; j < verts; j+=(stacks+1)) {
        if (j < verts-(stacks+1)) {
            for (int i = 0; i < stacks; i++)
                Torus::addIndex(j + i, j + i + 1, j + i + (stacks + 2));

            for (int i = 0; i < stacks; i++)
                Torus::addIndex(j + i + stacks + 2, j + i + (stacks + 1), j + i);
        } else {
            for (int i = 0; i < stacks; i++)
                Torus::addIndex(j+i,j+i+1,i+1);

            for (int i = 0; i < stacks; i++)
                Torus::addIndex(i+1,i,j+i);
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
