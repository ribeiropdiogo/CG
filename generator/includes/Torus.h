/**
 * Necessary methods to work with a torus.
 *
 * Torus is a subclass of Figure.
 */
#ifndef GENERATOR_TORUS_H
#define GENERATOR_TORUS_H

#include "Figure.h"
#include <glm/glm.hpp>

class Torus : public Figure {
public:
    /**
     * Generates a base torus with the specified inner and outer
     * radius, stacks and slice numbers.
     *
     * Stacks are along the y axis, and the slices sorround them.
     *
     * @param R Distance of the torus from the center.
     * @param r Radius of the torus strip.
     * @param stacks Number of stacks.
     * @param slices Number of slices.
     */
    Torus(float R, float r, int stacks, int slices);

private:
    /**
     * Used to calculate the normal vector at any given
     * point. The tangent correspondent to the crossproduct
     * between the partial derivatives du and dv.
     *
     * @param u U component of the point.
     * @param v V component of the point.
     * @param r Radius of the torus strip.
     * @param R Distance of the torus from the center.
     *
     * @return The normal to the point.
     */
    static glm::vec3 getGradient(float u, float v, float r, float R);
};

#endif
