//
// Created by syrayse on 05/04/20.
//

#ifndef GENERATOR_PATCHFIG_H
#define GENERATOR_PATCHFIG_H

#include "Figure.h"
#include "Vec3.h"
#include "BezierSuf.h"

class PatchFig : public Figure {
private:
    int n_patches{};
    BezierSuf * bs = nullptr;
    void loadFile(char *file);
    void buildPatches(int tessellation);
public:
    PatchFig(char *file, int tessellation);
};

#endif //GENERATOR_PATCHFIG_H
