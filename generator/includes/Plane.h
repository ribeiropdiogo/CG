#ifndef GENERATOR_PLANE_H
#define GENERATOR_PLANE_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include "Figure.h"

class Plane : public Figure {
private:
    glm::vec2 specs;

public:
    Plane(float width, float height);
    Plane(float width, float height, float ceil, std::string height_map);
};

#endif