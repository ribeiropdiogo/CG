//
// Created by syrayse on 09/03/20.
//
#include "DrawEvent.h"

DrawEvent::DrawEvent(unsigned int id, Object3d obj) {
    m_id = id;
    m_obj = obj;
    texture = "";
}

DrawEvent::DrawEvent(unsigned int id, Object3d obj,
                     float * materialOptions, string texture) : DrawEvent(id,obj) {
    if(!texture.empty()) {
        string prepend = "../../resources/textures/";
        prepend.append(texture);
        addTexture(prepend);
    }

    for(int i = 0; i < 13; i++) {
        material[i] = materialOptions[i];
    }
}

Object3d DrawEvent::getObj() {
    return m_obj;
}

unsigned int DrawEvent::getBufferId() {
    return m_id;
}

float * DrawEvent::getMaterialOptions() {
    return material;
}

void DrawEvent::addTexture(const string &n_texture) {
    texture = n_texture;
}

string DrawEvent::getTexture() {
    return texture;
}