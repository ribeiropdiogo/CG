//
// Created by syrayse on 09/03/20.
//
#include "DrawEvent.h"

DrawEvent::DrawEvent(Object3d obj) {
    m_obj = obj;
}

Object3d DrawEvent::getObj() {
    return m_obj;
}