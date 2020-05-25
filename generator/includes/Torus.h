#ifndef GENERATOR_TORUS_H
#define GENERATOR_TORUS_H

#include "Figure.h"
#include <glm/glm.hpp>

class Torus : public Figure{
public:
    Torus(float R, float r, int stacks, int slices);
private:
    static glm::vec3 getGradient(float u, float v, float r, float R);

};


#endif
