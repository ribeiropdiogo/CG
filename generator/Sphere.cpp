#include <cmath>
#include "Sphere.h"
Sphere::Sphere(float radius, int stacks, int slices) : Figure(stacks, slices) {
    double al = 2*M_PI / slices;
    double be = M_PI / stacks;
    int k1, k2, t1, t2, tmp;

    Sphere::addVertice(0,0,-radius);
    Figure::addNormal(0.0f, 0.0f, -1.0f);
    Figure::addTexCoord(0.0f, 0.0f);

    // Para cada stack
    for (int j =   1; j < stacks; j++) {

        // Para cada slices
        for (int i = 0; i < slices; i++) {
            Sphere::polarVertex(al, be, radius, i, j);
            Figure::addTexCoord((float)i / (float)slices,
                    (float)j / (float)stacks);
        }
    }

    tmp = Sphere::getVerticeSize() / 3;
    Sphere::addVertice(0,0,radius);
    Figure::addNormal(0.0f, 0.0f, 1.0f);
    Figure::addTexCoord(1.0f, 1.0f);

    for(int j = 0; j < stacks - 1; j++) {
        k1 = 1;
        for(int i = 0; i < slices - 1; i++,  k1++) {
            Figure::addIndex(0, k1, k1+1);
        }
    }
    Figure::addIndex(0, k1, 1);

    for(int j = 0; j < stacks - 1; j++) {
        k1 = tmp - stacks;
        for(int i = 0; i < slices - 1; i++,  k1++) {
            Figure::addIndex(tmp, k1+1, k1);
        }
    }
    Figure::addIndex(tmp, tmp - stacks, k1);

    for(int j = 0; j < stacks - 2 ; j++) {
        k1 = j * (stacks ) + 1;
        k2 = k1 + stacks ;

        for(int i = 0; i < slices  - 1; i++, k1++, k2++) {
            Figure::addIndex(k2 + 1, k1 + 1, k1);
            Figure::addIndex(k2, k2 + 1, k1);
        }

        t1 = j * (stacks ) + 1;
        t2 = t1 + stacks ;

        Figure::addIndex(t2, t1, k1);
        Figure::addIndex(k2, t2, k1);

    }
}

void Sphere::polarVertex(double al, double be, float radius, int i, int j) {
    float px = radius * sin(al * i) * cos(-M_PI_2 + be * j);
    float py = radius * cos(al * i) * cos(-M_PI_2 + be * j);
    float pz = radius * sin(-M_PI_2 +  be * j);
    float len = sqrtf(powf(px,2) + powf(py,2) + powf(pz,2));
    Figure::addVertice(px, py, pz);
    Figure::addNormal(px / len, py / len, pz / len);

}