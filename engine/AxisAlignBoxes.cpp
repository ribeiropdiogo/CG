//
// Created by Diogo Ribeiro on 02/04/2020.
//

#include "AxisAlignBoxes.h"
#include "Vec3.h"

AxisAlignBoxes::AxisAlignBoxes( Vec3 &corner,  float x, float y, float z) {

    setBox(corner,x,y,z);
}



AxisAlignBoxes::AxisAlignBoxes(void) {


    corner.m_x = 0; corner.m_y = 0; corner.m_z = 0;

    x = 1.0f;
    y = 1.0f;
    z = 1.0f;

}


AxisAlignBoxes::~AxisAlignBoxes() {}



void AxisAlignBoxes::setBox( Vec3 &corner,  float x, float y, float z) {


    this->corner.copy(corner);

    if (x < 0.0) {
        x = -x;
        this->corner.m_x -= x;
    }
    if (y < 0.0) {
        y = -y;
        this->corner.m_y -= y;
    }
    if (z < 0.0) {
        z = -z;
        this->corner.m_z -= z;
    }
    this->x = x;
    this->y = y;
    this->z = z;


}



Vec3 AxisAlignBoxes::getVertexP(Vec3 &normal) {

    Vec3 res = corner;

    if (normal.m_x > 0)
        res.m_x += x;

    if (normal.m_y > 0)
        res.m_y += y;

    if (normal.m_z > 0)
        res.m_z += z;

    return(res);
}



Vec3 AxisAlignBoxes::getVertexN(Vec3 &normal) {

    Vec3 res = corner;

    if (normal.m_x < 0)
        res.m_x += x;

    if (normal.m_y < 0)
        res.m_y += y;

    if (normal.m_z < 0)
        res.m_z += z;

    return(res);
}



