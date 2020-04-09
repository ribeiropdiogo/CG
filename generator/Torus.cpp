//
// Created by Diogo Ribeiro on 07/04/2020.
//
#include <cmath>
#include <iostream>
#include "Torus.h"

Torus::Torus(float innerRadius, float outerRadius, float height,int stacks, int slices) : Figure(stacks, slices) {
    double thetainc = 2*M_PI / slices;
    double phiinc = 2*M_PI / stacks;
    double theta = 0.0, phi = 0.0;
    int halfstacks = stacks/2;

    int thicc = height;

    double r = thicc/2;
    double R = innerRadius + ((outerRadius - innerRadius)/2);

    //cout<<"sides: "<<slices<<endl;

    for (int i = 0; i < slices; i++){
        phi = 0.0;
        //cout<<"anel "<<i+1<<endl;
        for (int j = 0; j <= stacks; j++){
            //cout<<"vertice "<<j<<endl;
            Torus::addVertice((R+r*cos(phi))*cos(theta),r*sin(phi),(R+r*cos(phi))*sin(theta));
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
