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
    diffR = diffG = diffB = 0.0f;
    texture = "";
}

DrawEvent::DrawEvent(unsigned int id, Object3d obj,
        int r, int g, int b,float diffR, float diffG, float diffB, string texture) : DrawEvent(id,obj) {
    addColor(r,g,b);
    addAmbiance(diffR,diffG,diffB);
    addTexture(texture);
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

void DrawEvent::addAmbiance(float n_diffR, float n_diffG, float n_diffB) {
    diffR = n_diffR;
    diffG = n_diffG;
    diffB = n_diffB;
}

void DrawEvent::addTexture(const string &n_texture) {
    texture = n_texture;
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

float DrawEvent::getDiffR() {
    return diffR;
}

float DrawEvent::getDiffG() {
    return diffG;
}

float DrawEvent::getDiffB() {
    return diffB;
}

string DrawEvent::getTexture() {
    return texture;
}