#include "Plane.h"

using namespace std;

Plane::Plane(float size) : Figure(1,1) {
    m_size = size;
    float half_size = size / 2;

    Figure::addVertice(-half_size, 0.0, -half_size);
    Figure::addVertice(half_size, 0.0, -half_size);
    Figure::addVertice(-half_size, 0.0, half_size);
    Figure::addVertice(half_size, 0.0, half_size);

    for(int i = 0; i < 4; i++)
        Figure::addNormal(0.0f, 1.0f, 0.0f);

    for(int i = 0; i <= 1; i++)
        for(int j = 0; j <= 1; j++)
            Figure::addTexCoord((float)i, (float) j);

    Figure::addIndex(0,2,3);
    Figure::addIndex(0,3,1);
}
