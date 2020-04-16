//
// Created by syrayse on 02/04/20.
//
#include "CRSpline.h"

CRSpline::CRSpline(vector<Vec3> points, bool loop) {
    int sz = points.size();
    isLoop = loop;
    m_points = *(new vector<Vec3>(sz));

    for(int i = 0; i < sz; i++) {
        m_points[i] = points[i];
        Vec3 P = points[i];
    }

    if(loop) {
        m_segments = sz;
    } else {
        m_segments = sz - 3;
    }

}

int CRSpline::getNSegments() {
    return m_segments;
}

bool CRSpline::isLooping() {
    return isLoop;
}

Vec3 CRSpline::getValueAt(float t) {
    int inds[4];
    getPointIndexes(t, inds);

    t = t - (float)(int)t;

    float q0, q1, q2, q3,
            tt = t * t, ttt = tt * t;

    q0 = -0.5f*ttt + tt - 0.5f*t;
    q1 = 1.5f*ttt - 2.5f*tt + 1;
    q2 = -1.5f*ttt + 2.0f*tt + 0.5f*t;
    q3 = 0.5f*ttt - 0.5f*tt;

    return ( m_points[inds[0]] * q0 ) + ( m_points[inds[1]] * q1 )
            + ( m_points[inds[2]] * q2 ) + ( m_points[inds[3]] * q3 );
}

Vec3 CRSpline::getGradientAt(float t) {
    int inds[4];
    getPointIndexes(t, inds);
    t = t - (float)(int)t;
    float q0, q1, q2, q3,
            tt = t * t, ttt = tt * t;
    q0 = -0.5f*(3*tt) + 2*t - 0.5f;
    q1 = 1.5f*(3*tt) - 2.5f*2*t;
    q2 = -1.5f*(3*tt) + 2.0f*2*t + 0.5f;
    q3 = 0.5f*(3*tt) - 0.5f*2*t;

    return ( m_points[inds[0]] * q0 ) + ( m_points[inds[1]] * q1 )
           + ( m_points[inds[2]] * q2 ) + ( m_points[inds[3]] * q3 );
}

void CRSpline::getPointIndexes(float t, int *ind) {
    int p0,p1,p2,p3, N = m_segments;

    if(!isLoop) {
        p1 = (int)t + 1;
        p2 = p1 + 1;
        p3 = p2 + 1;
        p0 = p1 - 1;
    } else {
        p1 = ((int)t) % N;
        p2 = (p1 + 1) % N;
        p3 = (p2 + 1) % N;
        p0 = (p1 - 1) % N;

        if(p0 < 0)
            p0 = N - 1;
    }

    ind[0] = p0;
    ind[1] = p1;
    ind[2] = p2;
    ind[3] = p3;
}