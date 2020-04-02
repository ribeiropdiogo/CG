//
// Created by syrayse on 17/03/20.
//

#ifndef GENERATOR_VEC3_H
#define GENERATOR_VEC3_H

class Vec3 {
public:
    float m_x, m_y, m_z;

    Vec3();
    Vec3(float x, float y, float z);
    Vec3(const Vec3 &v);
    Vec3 operator + (Vec3 const &vec);
    Vec3 operator - (Vec3 const &vec);
    Vec3 operator * (float num);
    float dotprod (Vec3 const &vec);
    Vec3 crossprod (Vec3 const &vec);
    float length();
    float getX();
    float getY();
    float getZ();
    void normalize();
    void copy(const Vec3 &v);
    float innerProduct(Vec3  &v);
    void set(float x,float y, float z);
};

#endif //GENERATOR_VEC3_H
