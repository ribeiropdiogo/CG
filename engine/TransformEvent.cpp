#include "TransformEvent.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <iostream>

#else
#include <GL/glew.h>
#include <GL/glut.h>

#include <utility>
#include <iostream>
#include <glm/vec3.hpp>
#include <mathAct.h>
#include <glm/gtc/type_ptr.hpp>

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

TransformEvent::TransformEvent(vector<glm::vec3> points, int laptime, bool loop)
        : TransformEvent(CATMULLROM, 0.0f, 0.0f, 0.0f, 0.0f, laptime){
    Yii = glm::vec3(0.0f, 1.0f, 0.0f);
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
    int N = 1;

    if(m_timeDep) {
        if(m_type == CATMULLROM)
            N = spline->getNSegments();

        factor = (float) (milis*N - m_oldtime) / (float) m_laptime;

        if(m_type == CATMULLROM && !spline->isLooping()
            && m_totaltime + factor >= (float)spline->getNSegments()) {
            m_totaltime = (float) spline->getNSegments() - 0.01f;

        } else if( !(m_type == CATMULLROM && spline->isLooping())
            && m_totaltime + factor > 1.0 && m_type != ROTATE) {
            m_totaltime = 1.0;

        } else
            m_totaltime += factor;

        m_oldtime = milis*N;
    }

    k = m_totaltime;

    switch (m_type) {
        case SCALE:
            mt::scale(glm::vec3(m_x, m_y, m_z)*k);
            break;
        case ROTATE:
            mt::rotate(k*m_angle * (M_PI / 180.0f), glm::vec3(m_x, m_y, m_z));
            break;
        case TRANSLATE:
            mt::translate(glm::vec3(m_x, m_y, m_z)*k);
            break;
        case CATMULLROM:
            dealWithCatmullR(k);
            break;
        default:;
    }
}

void TransformEvent::dealWithCatmullR(float milis) {
    // Mover ponto
    glm::vec3 P = spline->getValueAt(milis);

    // Rotação
    glm::vec3 Xi = glm::normalize(spline->getGradientAt(milis));
    glm::vec3 Zi = glm::normalize(glm::cross(Xi, Yii));
    glm::vec3 Yi = glm::normalize(glm::cross(Zi, Xi));

    Yii = glm::vec3(Yi);

    float M[16] = {
            Xi.x, Xi.y, Xi.z, 0.0f,
            Yi.x, Yi.y, Yi.z, 0.0f,
            Zi.x, Zi.y, Zi.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };

    mt::translate(P);
    mt::multMatix(glm::make_mat4(M));
}