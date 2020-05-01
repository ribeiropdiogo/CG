#include <iostream>
#include <Vec3.h>
#include "Box.h"

#define DIM 3

Box::Box(float wx, float wy, float wz) : Box(wx,wy,wz,1) {}

Box::Box(float wx, float wy, float wz,
        int divisions) : Figure(divisions, divisions) {

    int nVertices = 0;
    float normals[] = {0.0f, 0.0f, 0.0f};
    float weights[] = {wx, wy, wz};
    float steps[DIM];

    // Constroi os steps necessarios.
    for(int i = 0; i < DIM; i++)
        steps[i] = weights[i] / (float) divisions;
    
    // Constroi todas as laterais.
    for(float & normal : normals) {
        normal = 1.0f;
        nVertices += genLateral(normals,steps,divisions,nVertices);

        normal = -1.0f;
        nVertices += genLateral(normals,steps,divisions,nVertices);

        normal = 0.0f;
    }
}


int Box::genLateral(float *normals, float *steps,
               int divisions, int nVertices) {
    int k1, k2, nNew = 0;

    // Reconstroi os pesos
    float wts[] = {steps[0] * (float) divisions,
                   steps[1] * (float) divisions,
                   steps[2] * (float) divisions};

    // Cria vetor de steps
    Vec3 vstep = * new Vec3(steps);

    // Importa normals para forma de Vec3.
    Vec3 norm = * new Vec3(normals);

    // Calcula o vetor delta.
    float nx = normals[0];
    Vec3 dlt = kv * (-nx) + iv * (1 - abs(nx));

    // Calcula o vetor s.
    Vec3 s = (dlt.crossprod(norm)).normalize();

    // Define o ponto inicial.
    float *dltv = dlt.getVec();
    float *sv = s.getVec();
    float *nv = norm.getVec();
    int in = getIndex(nv), idlt = getIndex(dltv), is = getIndex(sv);
    Vec3 P0 = norm * (wts[in]/2) - dlt * (wts[idlt] / 2) - s * (wts[is] / 2);

    // Colocar todos os vertices.
    for(int j = 0; j <= divisions; j++) {

        for(int i = 0; i <= divisions; i++) {
            Vec3 Pi = vstep * ( (s * (float)i) + (dlt * (float)j) ) + P0;  
            Figure::addVertice(Pi.getX(), Pi.getY(), Pi.getZ());
            Figure::addNormal(normals[0], normals[1], normals[2]);
            Figure::addTexCoord((float) i / (float) divisions,
                    (float) j / (float) divisions);
            nNew++;
        }

    }

    // Colocar os respectivos indices.
    for(int i = 0; i < divisions; i++) {
        for(int j = 0; j < divisions; j++) {
            k1 = nVertices + (divisions+1)*i + j;
            k2 = k1 + divisions + 1;

            Figure::addIndex(k1,k2 + 1,k2);
            Figure::addIndex(k1+1,k2 + 1,k1);
        }
    }

    return nNew;
}

int Box::getIndex(float *v) {
    int i;

    for(i = 0; i < DIM && abs(v[i]) < 0.1f; i++)
        ;

    return i % DIM;
}