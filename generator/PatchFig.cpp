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
    cout << "Before load" << endl;
    // Inits data necessary for the patch proc.
    loadFile(file);
    cout << "After load" << endl;

    cout << "Before build" << endl;
    // Algorithm to generate indexes and vertices
    buildPatches(tessellation);
    cout << "After build" << endl;
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

        //cout << "i: " << i << "total of " << idx_patches[i].size() << endl;
    }

    inFile >> n_controlps;

    vector<Vec3> controlps(n_controlps);

    cout << "contrlps: " << n_controlps << endl;

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

    cout << "size is " << controlps.size() << endl;

    bs = new BezierSuf(idx_patches, controlps);
}

void PatchFig::buildPatches(int tessellation) {
    int i, j, k1 = 0, k2;
    float du, dv;
    du = dv = 1.0f / (float) tessellation;

    for(int id = 0; id < n_patches; id++) {
        //cout << "id: " << id << endl;
        k1 = (tessellation + 1)*(tessellation + 1)*id;

        for (j = 0; j < tessellation; j++) {
            for (i = 0; i < tessellation; i++) {
                //cout << "(i,j): (" << i << "," << j << ")" << endl;
                Vec3 p = bs->getBezierSufPoint(du*(float)i, dv*(float)j, id);
                //cout << "u: " << du*(float)i << ",v: " << dv*(float)j << endl;
                //cout << "x: " << p.getX() << ",y: " << p.getY() << ",z: " << p.getZ() << endl;
                //cout << "After bez suf p" << endl;
                Figure::addVertice(p.getX(), p.getY(), p.getZ());
            }
        }

        for (j = 0; j < tessellation - 1; j++) {
            for (i = 0; i < tessellation - 1; i++) {
                k1 += (tessellation+1)*j + i;
                k2 = k1 + tessellation + 1;

                Figure::addIndex(k1, k2, k2 + 1);
                Figure::addIndex(k1, k2 + 1, k1 + 1);
            }
        }
    }
}