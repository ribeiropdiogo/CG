//
// Created by syrayse on 09/03/20.
//
// 0 is a scale
// 1 is a rotation
// 2 is a translation

#ifndef GENERATOR_TRANSFORMEVENT_H
#define GENERATOR_TRANSFORMEVENT_H

#include "CRSpline.h"

enum TType {ROTATE, TRANSLATE, SCALE, CATMULLROM};

class TransformEvent {
private:
    TType m_type;
    bool m_timeDep;
    int m_oldtime, m_laptime;
    float m_x, m_y, m_z, m_angle, m_totaltime;
    glm::vec3 Yii;
    CRSpline *spline = nullptr;
    TransformEvent(TType type, float x, float y, float z,
                    float angle, int laptime);
    void dealWithCatmullR(float milis);
public:
    TransformEvent(TType type, float x, float y, float z);
    TransformEvent(vector<glm::vec3> points, int laptime, bool loop);
    TransformEvent(TType type, float x, float y, float z, int laptime);
    TransformEvent(float x, float y, float z, float angle);
    TransformEvent(float x, float y, float z, int laptime);
    void process(int milis);
    TType getType();
    void printItself();
};

#endif //GENERATOR_TRANSFORMEVENT_H
