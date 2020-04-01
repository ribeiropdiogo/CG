//
// Created by syrayse on 10/03/20.
//
// 0 is a scale
// 1 is a rotation
// 2 is a translation

#include "TransformEvent.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

TransformEvent::TransformEvent(int type, float x, float y, float z,
                                float angle, int laptime) {
    m_type = type;
    m_x = x;
    m_y = y;
    m_z = z;
    m_angle = angle;
    m_laptime = laptime;
    m_timeDep = laptime != 0;
    m_totaltime = laptime ? 0.0 : 1.0;
    m_oldtime = 0;
}

TransformEvent::TransformEvent(int type, float x, float y, float z)
    : TransformEvent(type, x, y, z, 0.0f, 0){}

TransformEvent::TransformEvent(int type, float x, float y, float z, int laptime)
    : TransformEvent(type, x, y, z, 0.0f, laptime){}

TransformEvent::TransformEvent(float x, float y, float z, float angle)
    : TransformEvent(1, x, y, z, angle, 0){}

TransformEvent::TransformEvent(float x, float y, float z, int laptime)
    : TransformEvent(1, x, y, z, 360.0, laptime){}

void TransformEvent::process(int milis) {
    float factor, k;

    if(m_timeDep) {
        factor = (float) (milis - m_oldtime) / (float) m_laptime;

        if(m_totaltime + factor > 1.0 && m_type != 1)
            m_totaltime = 1.0;
        else
            m_totaltime += factor;

        m_oldtime = milis;
    }

    k = m_totaltime;

    switch (m_type) {
        case 0:
            glScalef(k*m_x, k*m_y, k*m_z);
            break;
        case 1:
            glRotatef(k*m_angle, m_x, m_y, m_z);
            break;
        case 2:
            glTranslatef(k*m_x, k*m_y, k*m_z);
            break;
    }
}