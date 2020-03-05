#ifndef GENERATOR_PLANE_H
#define GENERATOR_PLANE_H

#include "Figure.h"

class Plane : public Figure {
private:
    float m_size;
public:
    Plane(float size);
};

#endif //GENERATOR_PLANE_H