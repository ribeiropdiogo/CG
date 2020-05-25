/**
 * Used to define the calculations associated with the
 * catmull-rom curve.
 */

#ifndef GENERATOR_CRSPLINE_H
#define GENERATOR_CRSPLINE_H

#include <vector>
#include <glm/glm.hpp>

using namespace std;

class CRSpline {
private:
    /**
     * Number of segments found in the curve,
     * this values depends on if the spline is looping or not.
     */
    int m_segments;

    /**
     * Indicates if the still is a looping curve or not,
     * by default, it is always considered a looping curve.
     */
    bool isLoop;

    /**
     * Set of all points that compose the original catmull-rom
     * curve.
     */
    vector<glm::vec3> m_points;

    /**
     * Uses the time passed as argument in order to discover
     * the segment in which the object is currently situated at.
     *
     * @param t Time of the system.
     * @param ind Indexes associated with the time.
     */
    void getPointIndexes(float t, int *ind);

public:
    /**
     * Constructor for the catmull-rom curve using, must
     * pass as argument all of the points composing said curve,
     * and indicate if it is either looping or non-looping.
     *
     * @param points Set of all points of the curve.
     * @param loop Indicates if the curve is looping or not.
     */
    CRSpline(vector<glm::vec3> points, bool loop);

    /**
     * Gets the number of existing segments defined
     * using the catmull-rom curve indicated.
     *
     * @return The number of segments in the curve.
     */
    int getNSegments();

    /**
     * Verifies if the curve is looping or non-looping.
     *
     * @return The looping value.
     */
    bool isLooping();

    /**
     * Indicates the position of the given curve in the
     * specified time.
     *
     * @param t Time in which to verify.
     * @return The position of the curve.
     */
    glm::vec3 getValueAt(float t);

    /**
     * Calculates the tangent of the curve in the
     * specified position indicated by the time.
     *
     * @param t Time in which to verify.
     * @return Tangent at the given point.
     */
    glm::vec3 getGradientAt(float t);
};

#endif
