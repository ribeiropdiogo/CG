#include <cmath>
#include "Sphere.h"

Sphere::Sphere(float radius, int stacks, int slices) : Figure() {
    double al = 2 * M_PI / slices;
    double be = M_PI / stacks;
    int k1, k2;

    // Para cada stack
    for (int j = 0; j <= stacks; j++) {

        // Para cada slices
        for (int i = 0; i <= slices; i++) {
            Sphere::polarVertex(al, be, radius, i, j);
            Figure::addTexCoord((float)i / (float)slices,
                    (float)j / (float)stacks);
        }
    }

    for(int j = 0; j < stacks ; j++) {
        k1 = j * (slices + 1);
        k2 = k1 + slices + 1;

        for(int i = 0; i < slices ; i++, k1++, k2++) {
            Figure::addIndex(k2 + 1, k1 +1, k1);
            Figure::addIndex(k2, k2 + 1, k1);
        }

    }
}

void Sphere::polarVertex(double al, double be, float radius, int i, int j) {
    float px = radius * sin(al * i) * cos(-M_PI_2 + be * j);
    float pz = radius * cos(al * i) * cos(-M_PI_2 + be * j);
    float py = -radius * sin(-M_PI_2 +  be * j);
    float len = sqrtf(powf(px,2) + powf(py,2) + powf(pz,2));
    Figure::addVertice(px, py, pz);
    Figure::addNormal(px / len, py / len, pz / len);

}