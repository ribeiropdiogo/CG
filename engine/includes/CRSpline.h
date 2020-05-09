//
// Created by syrayse on 02/04/20.
//

#ifndef GENERATOR_CRSPLINE_H
#define GENERATOR_CRSPLINE_H

#include <vector>
#include "Vec3.h"
#include <glm/glm.hpp>

using namespace std;

/**
 * Classe que concretiza a spline
 * de Catmull-Rom.
 */
class CRSpline {
private:
    int m_segments;
    bool isLoop;
    vector<glm::vec3> m_points;
    void getPointIndexes(float t, int *ind);
public:
    CRSpline(vector<glm::vec3> points, bool loop);
    int getNSegments();
    bool isLooping();
    glm::vec3 getValueAt(float t);
    glm::vec3 getGradientAt(float t);
};

#endif //GENERATOR_CRSPLINE_H
