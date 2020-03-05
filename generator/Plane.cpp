#include "Plane.h"

using namespace std;

Plane::Plane(float size) : Figure(1,1) {
    m_size = size;
    float half_size = size / 2;

    Figure::addVertice(-half_size, 0.0, -half_size);
    Figure::addVertice(half_size, 0.0, -half_size);
    Figure::addVertice(-half_size, 0.0, half_size);
    Figure::addVertice(half_size, 0.0, half_size);

    Figure::addIndex(0,2,3);
    Figure::addIndex(0,3,1);
}
