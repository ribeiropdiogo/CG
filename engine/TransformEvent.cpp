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

#include <utility>
#endif

TransformEvent::TransformEvent(TType type, float x, float y, float z,
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

TransformEvent::TransformEvent(vector<Vec3> points, int laptime, bool loop)
        : TransformEvent(CATMULLROM, 0.0f, 0.0f, 0.0f, 0.0f, laptime){
    Yii = new Vec3(0.0f, 1.0f, 0.0f);
    spline = new CRSpline(std::move(points), loop);
}

TransformEvent::TransformEvent(TType type, float x, float y, float z)
    : TransformEvent(type, x, y, z, 0.0f, 0){}

TransformEvent::TransformEvent(TType type, float x, float y, float z, int laptime)
    : TransformEvent(type, x, y, z, 0.0f, laptime){}

TransformEvent::TransformEvent(float x, float y, float z, float angle)
    : TransformEvent(ROTATE, x, y, z, angle, 0){}

TransformEvent::TransformEvent(float x, float y, float z, int laptime)
    : TransformEvent(ROTATE, x, y, z, 360.0, laptime){}

void TransformEvent::process(int milis) {
    float factor, k;

    if(m_timeDep) {
        factor = (float) (milis - m_oldtime) / (float) m_laptime;

        if(m_type == CATMULLROM && !spline->isLooping()
            && m_totaltime + factor >= (float)spline->getNSegments())
            m_totaltime = (float) spline->getNSegments() - 0.01f;
        else if(m_totaltime + factor > 1.0 && m_type != ROTATE)
            m_totaltime = 1.0;
        else
            m_totaltime += factor;

        m_oldtime = milis;
    }

    k = m_totaltime;

    switch (m_type) {
        case SCALE:
            glScalef(k*m_x, k*m_y, k*m_z);
            break;
        case ROTATE:
            glRotatef(k*m_angle, m_x, m_y, m_z);
            break;
        case TRANSLATE:
            glTranslatef(k*m_x, k*m_y, k*m_z);
            break;
        case CATMULLROM:
            dealWithCatmullR(k);
            break;
        default:;
    }
}

void TransformEvent::dealWithCatmullR(float milis) {
    // Mover ponto
    Vec3 P = spline->getValueAt(milis);
    glTranslatef(P.getX(), P.getY(), P.getZ());

    // Rotação
    Vec3 Xi = (spline->getGradientAt(milis)).normalize();
    Vec3 Zi = (Xi.crossprod(*Yii)).normalize();
    Vec3 Yi = (Zi.crossprod(Xi)).normalize();
    Yii = &Yi;
    float M[16] = {
            Xi.getX(),  Xi.getY(),  Xi.getZ(),  0.0f,
            Yi.getX(),  Yi.getY(),  Yi.getZ(),  0.0f,
            Zi.getX(),  Zi.getY(),  Zi.getZ(),  0.0f,
            0.0f,       0.0f,       0.0f,       1.0f
    };

    glMultMatrixf(M);
}