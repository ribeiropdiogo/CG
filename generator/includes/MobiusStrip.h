/**
 * Necessary methods to work with mobius strip.
 *
 * Mobius Strip is a subclass of Figure.
 */
#ifndef GENERATOR_MOBIUSSTRIP_H
#define GENERATOR_MOBIUSSTRIP_H

#include "Figure.h"
#include <glm/glm.hpp>

class MobiusStrip : public Figure {
public:
    /**
     * Generates a mobius strip with the specified number
     * of stacks and slices.
     *
     * Stacks are along the y axis, and the slices sorround them.
     *
     * @param stacks Number of stacks.
     * @param slices Number of slices.
     */
    MobiusStrip(int stacks, int slices);

private:
    /**
     * Used to calculate the normal vector at any given
     * point. The tangent correspondent to the crossproduct
     * between the partial derivatives du and dv.
     *
     * @param u U component of the point.
     * @param v V component of the point.
     *
     * @return The normal to the point.
     */
    static glm::vec3 getGradient(float u, float v);

    /**
     * Used to calculate the point with the given
     * arguments.
     *
     * @param u U Component of the point.
     * @param v V Component of the point.
     *
     * @return Point calculated.
     */
    static glm::vec3 getPoint(float u, float v);
};

#endif
