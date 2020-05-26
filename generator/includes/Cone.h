/**
 * Necessary methods to work with cones.
 *
 * Cone is a subclass of Figure.
 */
#ifndef GENERATOR_CONE_H
#define GENERATOR_CONE_H

#include <glm/glm.hpp>
#include "Figure.h"

class Cone : public Figure {
public:
    /**
     * Generates a base cone with the specified radius, height,
     * and stacks and slice number.
     *
     * Stacks are along the y axis, and the slices sorround them.
     *
     * @param radius Radius of the figure.
     * @param height Height of the cone.
     * @param stacks Number of stacks defined.
     * @param slices Number of slices defined.
     */
    Cone(float radius, float height, int stacks, int slices);

private:
    /**
     * Used to specify polar coordinates, having into account
     * the ratio between the radius and height.
     *
     * @param al Alpha angle with the zy plane.
     * @param radius Radius of the cone stack.
     * @param sz Height of the stack slice.
     * @param j Number of the slice.
     * @param ratio Ratio between the radius and height.
     */
    void polarVertex(float al, float radius, float sz, int j, float ratio);

    /**
     * Calculates the normal vector to the surface at given
     * point, which depends on the angle.
     *
     * @param angle Alpha angle with the zy planes.
     * @param c Ratio between the radius and height.
     *
     * @return Vector normal to the surface.
     */
    static glm::vec3 getGradient(float angle, float c);
};

#endif
