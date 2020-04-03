//
// Created by syrayse on 02/04/20.
//

#ifndef GENERATOR_CRSPLINE_H
#define GENERATOR_CRSPLINE_H

#include <vector>
#include "Vec3.h"

using namespace std;

/**
 * Classe que concretiza a spline
 * de Catmull-Rom.
 */
class CRSpline {
private:
    int m_segments;
    bool isLoop;
    vector<Vec3> m_points;
    void getPointIndexes(float t, int *ind);
public:
    CRSpline(vector<Vec3> points, bool loop);
    int getNSegments();
    bool isLooping();
    Vec3 getValueAt(float t);
    Vec3 getGradientAt(float t);
};

#endif //GENERATOR_CRSPLINE_H
