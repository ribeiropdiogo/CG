#include <iostream>
#include <fstream>
#include <math.h>
#include <glm/glm.hpp>
#include "Cone.h"

using namespace std;

Cone::Cone(float radius, float height, int stacks, int slices)
    : Figure(stacks, slices) {
    float oneSlice = (M_PI * 2.0f) / slices;
    float oneStack = height / (float) stacks;
    float oneRadius = radius / (float) stacks;
    float currentRadius = radius;
    float x, z;
    float ratio = 1 / (radius / height);
    int i, k1, k2;

    Figure::addVertice(0.0, 0.0, 0.0);
    Figure::addNormal(0.0f, -1.0f, 0.0f);
    Figure::addTexCoord(0.0f, 0.0f);

    // Gera base
    for (i = 0; i < slices; i++) {
        z = radius * cos(oneSlice * (float) i);
        x = radius * sin(oneSlice * (float) i);
        Figure::addVertice(x, 0.0f, z);
        Figure::addNormal(0.0f, -1.0f, 0.0f);
        Figure::addTexCoord(z / radius, x / radius);
    }

    // Gera lateral
    for (i = 0; i <= stacks; i++) {

        float currentHeight = oneStack * i;

        for (int j = 0; j <= slices; j++) {
            polarVertex(oneSlice, currentRadius, currentHeight, j, ratio);
            Figure::addTexCoord((float) j / (float) slices,
                                (float) i / (float) stacks);
        }
        currentRadius -= oneRadius;
    }

    // Indexa base
    for (i = 1; i < slices; i++) {
        Figure::addIndex(0, i + 1, i);
    }
    Figure::addIndex(0, 1, i);

    // Indexa lateral
    for (i = 0; i < stacks; i++) {
        k1 = slices + i * (slices + 1);
        k2 = k1 + slices + 1;

        for (int j = 0; j <= slices; j++, k1++, k2++) {
            Figure::addIndex(k1, k1 + 1, k2);
            Figure::addIndex(k2, k1 + 1, k2 + 1);
        }

    }
}

void Cone::polarVertex(float al, float radius, float sz, int j,float ratio) {
    float angle = al * (float) j;
    float px = radius * sin(angle);
    float py = sz;
    float pz = radius * cos(angle);
    glm::vec3 grad = glm::normalize(Cone::getGradient(angle, ratio));
    Figure::addVertice(px, py, pz);
    Figure::addNormal(grad.x, grad.y, grad.z);
}

glm::vec3 Cone::getGradient(float angle, float c) {
    return glm::vec3(c * sin(angle), 1.0f, c * cos(angle));
}