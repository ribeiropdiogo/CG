//
// Created by angelo on 28/02/20.
//
#include <iostream>
#include <fstream>
#include <math.h>
#include "Cone.h"

using namespace std;

Cone::Cone(float radius, float height, int stacks, int slices)
    : Figure(stacks, slices) {
    float oneSlice = (M_PI * 2.0f) / slices;
    float oneStack = height / (float) stacks;
    float oneRadius = radius / (float) stacks;
    float currentRadius = radius;
    float x, z;
    int i,t1, t2, k1, k2, count;

    Figure::addVertice(0.0, 0.0, 0.0);

    // Gera base
    for(i = 0; i < slices; i++) {
        z = radius*cos(oneSlice*(float)i);
        x = radius*sin(oneSlice*(float)i);
        Figure::addVertice(x, 0.0f, z);
    }

    // Gera lateral
    for(i = 0; i < stacks; i++){

        float currentHeight = oneStack * i;

        for(int j = 0; j < slices; j++){
            polarVertex(oneSlice, oneSlice, currentRadius, currentHeight, j, j);
        }
        currentRadius -= oneRadius;
    }

    count = Figure::getVerticeSize() / 3;
    Figure::addVertice(0.0, height, 0.0);

    // Indexa base
    for(i = 1; i < slices; i++) {
        Figure::addIndex(0, i + 1, i);
    }
    Figure::addIndex(0, 1, i);

    // Indexa lateral
    for(i = 0; i < stacks - 1; i++) {
        t1 = k1 = slices + i*slices + 1;
        t2 = k2 = k1 + slices;

        for(int j = 0; j < slices  - 1; j++, k1++, k2++) {
            Figure::addIndex(k1, k1 + 1, k2);
            Figure::addIndex(k2, k1 + 1, k2 + 1);
        }

        Figure::addIndex(k1, t1, k2);
        Figure::addIndex(k2, t1, t2);
    }

    t1 = count - slices;
    for(i = 0; i < slices; i++) {
        Figure::addIndex(t1 + i, t1 + i + 1, count);
    }
    Figure::addIndex(count - 1, t1, count);
}



void Cone::polarVertex(double al, double be, float radius, float sz, int i, int j) {
    float px = radius * sin(al * i);
    float py = sz;
    float pz = radius * cos(be * j);
    Figure::addVertice(px, py, pz);
}