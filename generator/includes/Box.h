#ifndef GENERATOR_BOX_H
#define GENERATOR_BOX_H

#include "Figure.h"
#include <glm/glm.hpp>

using namespace glm;

class Box : public Figure {
public:
    Box(float wx, float wy, float wz);

    Box(float wx, float wy, float wz,
        int divisions, bool uvMask);

private:
    glm::vec3 kv = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 iv = glm::vec3(1.0f, 0.0f, 0.0f);

    int genLateral(float *normals, float *steps,
                   int divisions, int nVertices, bool uvMask);

    static int getIndex(float *v);

    void addPersonalUVMask(int divisions);

    void genUVMask(vec2 pos, vec2 du, vec2 dv, int divisions);
};

#endif