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

TransformEvent::TransformEvent(int type, float x, float y, float z)
    : TransformEvent(type, x, y, z, 0.0f){}

TransformEvent::TransformEvent(int type, float x, float y, float z, float angle) {
    m_type = type;
    m_x = x;
    m_y = y;
    m_z = z;
    m_angle = angle;
}

void TransformEvent::process() {
    switch (m_type) {
        case 0:
            glScalef(m_x, m_y, m_z);
            break;
        case 1:
            glRotatef(m_angle, m_x, m_y, m_z);
            break;
        case 2:
            glTranslatef(m_x, m_y, m_z);
            break;
    }
}