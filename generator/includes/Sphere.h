/**
 * Necessary methods to work with a sphere.
 *
 * Sphere is a subclass of Figure.
 */
#ifndef GENERATOR_SPHERE_H
#define GENERATOR_SPHERE_H

#include "Figure.h"

class Sphere : public Figure {
public:
    /**
     * Generates a base sphere with the specified radius and stacks and slice number.
     *
     * Stacks are along the y axis, and the slices sorround them.
     *
     * @param radius Radius of the figure.
     * @param stacks Number of stacks defined.
     * @param slices Number of slices defined.
     */
    Sphere(float radius, int stacks, int slices);

private:
    /**
     * Used to specify polar vertex coordinates.
     *
     * @param al Alpha angle with the zy plane.
     * @param be Beta angle with the zx plane.
     * @param radius Radius of the figure.
     * @param i Slices number.
     * @param j Stack number.
     */
    void polarVertex(double al, double be, float radius, int i, int j);
};

#endif