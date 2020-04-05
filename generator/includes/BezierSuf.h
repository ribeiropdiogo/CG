//
// Created by syrayse on 05/04/20.
//

#ifndef GENERATOR_BEZIERSUF_H
#define GENERATOR_BEZIERSUF_H

#include <vector>
#include "Vec3.h"

using namespace std;

class BezierSuf {
private:
    float Mat[16] = {
        -1.0f,  3.0f,   -3.0f,  1.0f,
        3.0f,   -6.0f,  3.0f,   0.0f,
        -3.0f,  3.0f,   0.0f,   0.0f,
        1.0f,   0.0f,   0.0f,   0.0f
    };
    vector<vector<int>> idx_patches;        // Indices of each Patch
    vector<Vec3> controlps;                 // Control Points
    Vec3 multMatrixes(const float *alphas, const float *betas, int idPatch);
    void multLineM(float *line);
    Vec3 getBezierU(float u, float v, int idPatch);
    Vec3 getBezierV(float u, float v, int idPatch);
    float *getCubic(float n);
    float *getCubicDeriv(float n);
public:
    BezierSuf(vector<vector<int>> n_idx_patches,
              vector<Vec3> n_controlps);
    Vec3 getBezierSufPoint(float u, float v, int idPatch);
    Vec3 getBezierSufNorm(float u, float v, int idPatch);
};

#endif //GENERATOR_BEZIERSUF_H
