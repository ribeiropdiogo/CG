//
// Created by syrayse on 09/03/20.
//

#ifndef GENERATOR_DRAWEVENT_H
#define GENERATOR_DRAWEVENT_H

#include "Object3d.h"
#include <GL/glew.h>

class DrawEvent  {
private:
    Object3d m_obj;
public:
    DrawEvent(Object3d obj);
    Object3d getObj();
};

#endif //GENERATOR_DRAWEVENT_H
