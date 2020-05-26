/**
 * Necessary methods to generate planes.
 *
 * Plane is a subclass of Figure.
 */
#ifndef GENERATOR_PLANE_H
#define GENERATOR_PLANE_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include "Figure.h"

class Plane : public Figure {
private:
    /**
     * Contains the specification of the plane, its width
     * and height.
     */
    glm::vec2 specs;

public:
    /**
     * Generates a base plane, with just one division and the
     * specified width and height.
     *
     * @param width Width of the plane.
     * @param height Height of the plane.
     */
    Plane(float width, float height);

    /**
     * Generates a plane using a height map as input,
     * the number of divisions will be thus associated with
     * the width and height of the heigh_map.
     *
     * @param width Width of the plane.
     * @param height Height of the plane.
     * @param ceil Maximum high defined by the height map.
     * @param height_map Input height map being used.
     */
    Plane(float width, float height, float ceil, std::string height_map);
};

#endif