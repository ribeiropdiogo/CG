//
// Created by syrayse on 09/03/20.
//
// 0 is a scale
// 1 is a rotation
// 2 is a translation

#ifndef GENERATOR_TRANSFORMEVENT_H
#define GENERATOR_TRANSFORMEVENT_H

class TransformEvent {
private:
    int m_type;
    float m_x, m_y, m_z, m_angle;
public:
    TransformEvent(int type, float x, float y, float z);
    TransformEvent(int type, float x, float y, float z, float angle);
    void process();
public:
};

#endif //GENERATOR_TRANSFORMEVENT_H
