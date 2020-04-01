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
    bool m_timeDep;
    int m_type, m_oldtime, m_laptime;
    float m_x, m_y, m_z, m_angle, m_totaltime;
    TransformEvent(int type, float x, float y, float z,
                    float angle, int laptime);
public:
    TransformEvent(int type, float x, float y, float z);
    TransformEvent(int type, float x, float y, float z, int laptime);
    TransformEvent(float x, float y, float z, float angle);
    TransformEvent(float x, float y, float z, int laptime);
    void process(int milis);
};

#endif //GENERATOR_TRANSFORMEVENT_H
