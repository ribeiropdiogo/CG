//
// Created by syrayse on 05/04/20.
//

#ifndef GENERATOR_PATCHFIG_H
#define GENERATOR_PATCHFIG_H

#include "Figure.h"
#include "Vec3.h"

class PatchFig : public Figure {
private:
    int n_patches{},                          // No of Patches
        n_controlps{};                        // No of control points
    vector<vector<int>> idx_patches;        // Indices of each Patch
    vector<Vec3> controlps;                 // Control Points
    void loadFile(char *file);
public:
    PatchFig(char *file);
};

#endif //GENERATOR_PATCHFIG_H
