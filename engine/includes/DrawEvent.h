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
    int r, g, b;
    float diffR, diffG, diffB;
    string texture;
public:
    DrawEvent(unsigned int id, Object3d obj);
    Object3d getObj();
    unsigned int getBufferId();

    void addColor(int red, int green, int blue);

    void addAmbiance(float n_diffR, float n_diffG, float n_diffB);

    void addTexture(const string &n_texture);

    int getGreen();

    int getRed();

    int getBlue();

    float getDiffR();

    float getDiffG();

    float getDiffB();

    string getTexture();
};

#endif //GENERATOR_DRAWEVENT_H
