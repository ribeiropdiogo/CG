//
// Created by syrayse on 09/03/20.
//

#ifndef GENERATOR_DRAWEVENT_H
#define GENERATOR_DRAWEVENT_H

#ifdef __APPLE__
#else
#include <GL/glew.h>
#endif
#include "Object3d.h"

class DrawEvent  {
private:
    unsigned int m_id;
    Object3d m_obj;
public:
    DrawEvent(unsigned int id, Object3d obj);
    Object3d getObj();
    unsigned int getBufferId();
};

#endif //GENERATOR_DRAWEVENT_H
