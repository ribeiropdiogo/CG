#ifndef GENERATOR_CONE_H
#define GENERATOR_CONE_H

#include <glm/glm.hpp>
#include "Figure.h"

class Cone : public Figure {
public:
    Cone(float radius, float height, int stacks, int slices);

private:
    void polarVertex(float al, float radius, float sz, int j, float ratio);

    static glm::vec3 getGradient(float angle, float c);
};

#endif
