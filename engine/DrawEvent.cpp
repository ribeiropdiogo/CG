//
// Created by syrayse on 09/03/20.
//
#include "DrawEvent.h"

DrawEvent::DrawEvent(unsigned int id, Object3d * obj) {
    m_id = id;
    m_obj = obj;
}

Object3d * DrawEvent::getObj() {
    return m_obj;
}

unsigned int DrawEvent::getBufferId() {
    return m_id;
}