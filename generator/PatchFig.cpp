//
// Created by syrayse on 05/04/20.
//

#include "PatchFig.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

PatchFig::PatchFig(char *file, int tessellation)
    : Figure(0, 0) {
    // Inits data necessary for the patch proc.
    loadFile(file);

    // Algorithm to generate indexes and vertices
    buildPatches(tessellation);
}

void PatchFig::loadFile(char *file) {
    int n_controlps, j;
    float ps[3];
    string line, lineElem;

    ifstream inFile(file);
    inFile >> n_patches;

    vector<vector<int>> idx_patches(n_patches);

    getline(inFile, line);

    for(int i = 0; i < n_patches; i++) {
        getline(inFile, line);

        istringstream lineStream(line);

        while( getline(lineStream, lineElem, ',') ) {
            idx_patches[i].push_back(stoi(lineElem));
        }
    }

    inFile >> n_controlps;

    vector<Vec3> controlps(n_controlps);

    getline(inFile, line);

    for(int i = 0; i < n_controlps; i++) {
        j = 0;
        getline(inFile, line);

        istringstream lineStream(line);

        while( j < 3 && getline(lineStream, lineElem, ',') ) {
            ps[j] = stof(lineElem);
            j++;
        }

        controlps[i] = *new Vec3(ps[0], ps[1], ps[2]);
    }

    inFile.close();

    bs = new BezierSuf(idx_patches, controlps);
}

void PatchFig::buildPatches(int tessellation) {
    int i, j, k1 = 0, k2, tmp;
    float du, dv;
    du = dv = 1.0f / (float) tessellation;

    for(int id = 0; id < n_patches; id++) {

        tmp = (tessellation + 1)*(tessellation + 1)*id;

        for (j = 0; j <= tessellation; j++) {
            for (i = 0; i <= tessellation; i++) {
                Vec3 p = bs->getBezierSufPoint(du*(float)i, dv*(float)j, id);

                Figure::addVertice(p.getX(), p.getY(), p.getZ());
            }
        }

        for (j = 0; j < tessellation; j++) {
            for (i = 0; i < tessellation; i++) {
                k1 = tmp + (tessellation+1)*j + i;
                k2 = k1 + tessellation + 1;

                Figure::addIndex(k1, k2 + 1, k2 );
                Figure::addIndex(k1, k1 + 1, k2 + 1);
            }
        }
    }
}