//
// Created by syrayse on 09/03/20.
//
#include "DrawEvent.h"

DrawEvent::DrawEvent(unsigned int id, Object3d obj) {
    m_id = id;
    m_obj = obj;
    r = 0;
    g = 0;
    b = 255;
}

Object3d DrawEvent::getObj() {
    return m_obj;
}

unsigned int DrawEvent::getBufferId() {
    return m_id;
}

void DrawEvent::addColor(int red, int green, int blue) {
    r = red;
    g = green;
    b = blue;
}

int DrawEvent::getRed(){
    return r;
}

int DrawEvent::getGreen(){
    return g;
}

int DrawEvent::getBlue(){
    return b;
}