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
    string texture;
    float material[13] = {
            0.0f, 0.0f, 0.0f, 1.0f, // Diffuse
            0.0f, 0.0f, 0.0f, 1.0f, // Ambient
            1.0f, 1.0f, 1.0f, 1.0f, // Specular
            0.0f                    // Shininess
    };
public:
    DrawEvent(unsigned int id, Object3d obj);
    DrawEvent(unsigned int id, Object3d obj, float * materialOptions, string texture);
    Object3d getObj();
    unsigned int getBufferId();

    void addTexture(const string &n_texture);

    float * getMaterialOptions();

    string getTexture();
};

#endif //GENERATOR_DRAWEVENT_H
