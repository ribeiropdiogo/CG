#ifndef GENERATOR_MOBIUSSTRIP_H
#define GENERATOR_MOBIUSSTRIP_H

#include "Figure.h"
#include <glm/glm.hpp>

class MobiusStrip : public Figure {
public:
    MobiusStrip(int stacks, int slices);

private:
    static glm::vec3 getGradient(float u, float v);

    static glm::vec3 getPoint(float u, float v);
};

#endif
