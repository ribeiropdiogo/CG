//
// Created by syrayse on 17/03/20.
//

#ifndef GENERATOR_VEC3_H
#define GENERATOR_VEC3_H

class Vec3 {
private:
    float m_x, m_y, m_z;
public:
    Vec3();
    Vec3(float x, float y, float z);
    Vec3 operator + (Vec3 const &vec);
    Vec3 operator - (Vec3 const &vec);
    Vec3 operator * (float num);
    float dotprod (Vec3 const &vec);
    Vec3 crossprod (Vec3 const &vec);
    float getX();
    float getY();
    float getZ();
    Vec3 normalize();
    float length();
};

#endif //GENERATOR_VEC3_H
