/**
 * Necessary methods to work with a box.
 *
 * Box is a subclass of Figure.
 */
#ifndef GENERATOR_BOX_H
#define GENERATOR_BOX_H

#include "Figure.h"
#include <glm/glm.hpp>

using namespace glm;

class Box : public Figure {
public:
    /**
     * Used to generate a base box centered at the
     * original referential. This box is built with exactly
     * 1 division.
     *
     * @param wx Length of the box in x component.
     * @param wy Length of the box in y component.
     * @param wz Length of the box in z component.
     */
    Box(float wx, float wy, float wz);

    /**
     * Used to generate a base box centered at the
     * original referential. According with the specified
     * number of divisions, having into account
     * that we may want to specify this box to be able to
     * adapt to atlas textures.
     *
     * @param wx Length of the box in x component.
     * @param wy Length of the box in y component.
     * @param wz Length of the box in z component.
     * @param divisions Number of box divisions.
     * @param uvMask true if using atlas texture, false otherwise.
     */
    Box(float wx, float wy, float wz,
        int divisions, bool uvMask);

private:
    /**
     * Vector used internally to process division based box.
     * See phase 4 report for more information.
     */
    glm::vec3 kv = glm::vec3(0.0f, 0.0f, 1.0f);

    /**
     * Vector used internally to process division based box.
     * See phase 4 report for more information.
     */
    glm::vec3 iv = glm::vec3(1.0f, 0.0f, 0.0f);

    /**
     * Generates a lateral face, using the normal vector
     * to identify which of the normal is being dealt with.
     *
     * @param normals Normal vector to the surface.
     * @param steps The step given in each component, according to divisions.
     * @param divisions Number of divisions.
     * @param nVertices Number of vertices previously generated.
     * @param uvMask true if using atlas texture, false otherwise.
     *
     * @return Number of newly generated vertexs.
     */
    int genLateral(float *normals, float *steps,
                   int divisions, int nVertices, bool uvMask);

    /**
     * Used in synchrony with the normal vector in order to identify
     * which step must be used in said face.
     *
     * @param v Normal vector.
     *
     * @return Identifies first non-zero component of the normal vector.
     */
    static int getIndex(float *v);

    /**
     * Generates all of the textures coordinates in case we
     * seek to construct boxes to adapt to atlas textures.
     *
     * @param divisions Number of divisions.
     */
    void addPersonalUVMask(int divisions);

    /**
     * Generates the texture coordinates associated
     * with one of the cubes faces. The values passed as
     * argument are direct mappings between the image points
     * representing each face.
     *
     * @param pos Start position in the image.
     * @param du Step size in the horizontal direction.
     * @param dv Step size in the vertical direction.
     * @param divisions Number of divisions.
     */
    void genUVMask(vec2 pos, vec2 du, vec2 dv, int divisions);
};

#endif