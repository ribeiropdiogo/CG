#ifndef GENERATOR_BEZIERSUF_H
#define GENERATOR_BEZIERSUF_H

#include <vector>
#include <glm/glm.hpp>

using namespace std;

class BezierSuf {
private:
    float Mat[16] = {
            -1.0f, 3.0f, -3.0f, 1.0f,
            3.0f, -6.0f, 3.0f, 0.0f,
            -3.0f, 3.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f
    };
    vector<vector<int>> idx_patches;
    vector<glm::vec3> controlps;

    glm::vec3 multMatrixes(const float *alphas, const float *betas, int idPatch);

    void multLineM(float *line);

    glm::vec3 getBezierU(float u, float v, int idPatch);

    glm::vec3 getBezierV(float u, float v, int idPatch);

    float *getCubic(float n);

    float *getCubicDeriv(float n);

public:
    BezierSuf(vector<vector<int>> n_idx_patches,
              vector<glm::vec3> n_controlps);

    glm::vec3 getBezierSufPoint(float u, float v, int idPatch);

    glm::vec3 getBezierSufNorm(float u, float v, int idPatch);
};

#endif
