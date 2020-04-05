//
// Created by syrayse on 05/04/20.
//

#include "PatchFig.h"
#include <fstream>
#include <sstream>

PatchFig::PatchFig(char *file)
    : Figure(0, 0) {
    // Inits data necessary for the patch proc.
    loadFile(file);

    // Algorithm to generate indexes and vertices
    // Insert here
}

void PatchFig::loadFile(char *file) {
    float x, y, z;
    string line, lineElem;
    ifstream inFile(file);
    inFile >> n_patches;

    for(int i = 0; i < n_patches; i++) {
        getline(inFile, line);

        istringstream lineStream(line);

        while( getline(lineStream, lineElem, ',') ) {
            idx_patches[i].push_back(stoi(lineElem));
        }
    }

    inFile >> n_controlps;

    for(int i = 0; i < n_controlps; i++) {
        inFile >> x >> y >> z;
        controlps.push_back(*new Vec3(x, y, z));
    }

    inFile.close();
}