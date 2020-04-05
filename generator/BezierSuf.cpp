//
// Created by syrayse on 05/04/20.
//
#include "BezierSuf.h"
#include <cmath>
#include <iostream>

#define getM(i,j,m) (m[((4*i)+j)])

BezierSuf::BezierSuf(vector<vector<int>> n_idx_patches,
                        vector<Vec3> n_controlps) {
    idx_patches = std::move(n_idx_patches);
    controlps = std::move(n_controlps);
}

void BezierSuf::multLineM(float *line) {
    float calc[4];

    for(int i = 0; i < 4; i++) {
        calc[i] = 0.0f;

        for(int j = 0; j < 4; j++) {
            calc[i] += line[j] * getM(j, i, Mat);
        }
    }

    for(int i = 0; i < 4; i++)
        line[i] = calc[i];
}

Vec3 BezierSuf::getBezierU(float u, float v, int idPatch) {
    return multMatrixes(getCubicDeriv(u), getCubic(v), idPatch);
}

Vec3 BezierSuf::getBezierV(float u, float v, int idPatch) {
    return multMatrixes(getCubic(u), getCubicDeriv(v), idPatch);
}

Vec3 BezierSuf::multMatrixes(const float *alphas, const float *betas, int idPatch) {
    Vec3 result;
    vector<int> patch = idx_patches[idPatch];
    float beta, alpha;

    for(int j = 0; j < 4; j++) {
        Vec3 tmp;
        beta = betas[j];

        for(int i = 0; i < 4; i++) {
            alpha = alphas[i];
            tmp = tmp + (controlps[getM(j,i,patch)] * alpha);
        }

        result = result + (tmp * beta);
    }

    return result;
}

Vec3 BezierSuf::getBezierSufPoint(float u, float v, int idPatch) {
    return multMatrixes(getCubic(u), getCubic(v), idPatch);
}

Vec3 BezierSuf::getBezierSufNorm(float u, float v, int idPatch) {
    Vec3 bu = getBezierU(u, v, idPatch);
    Vec3 bv = getBezierV(u, v, idPatch);

    return (bu.crossprod(bv)).normalize();
}

float* BezierSuf::getCubic(float n) {
    auto *r = new float[4]{pow(n,3.0f), pow(n,2.0f), n, 1.0f};

    multLineM(r);

    return r;
}

float* BezierSuf::getCubicDeriv(float n) {
    auto *r = new float[4]{3.0f*pow(n,2.0f), 2*n, 1.0f, 0.0f};

    multLineM(r);

    return r;
}