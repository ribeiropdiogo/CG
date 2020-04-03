#include "Vec3.h"
#include <math.h>

Vec3::Vec3() : Vec3(0.0f, 0.0f, 0.0f) {}

Vec3::Vec3(float x, float y, float z) {
    m_x = x;
    m_y = y;
    m_z = z;
}

Vec3 Vec3::operator + (Vec3 const &vec) {
    return {m_x + vec.m_x, m_y + vec.m_y, m_z + vec.m_z};
}

Vec3 Vec3::operator - (Vec3 const &vec) {
    return {m_x - vec.m_x, m_y - vec.m_y, m_z - vec.m_z};
}

Vec3 Vec3::operator * (float num) {
    return {m_x*num, m_y*num, m_z*num};
}

float Vec3::dotprod(Vec3 const &vec) {
    return m_x*vec.m_x + m_y*vec.m_y + m_z*vec.m_z;
}

Vec3 Vec3::crossprod (Vec3 const &vec) {
    return {m_y*vec.m_z - m_z*vec.m_y,
            m_z*vec.m_x - m_x*vec.m_z,
            m_x*vec.m_y - m_y*vec.m_x};
}

float Vec3::getX() {
    return m_x;
}

float Vec3::getY() {
    return m_y;
}

float Vec3::getZ() {
    return m_z;
}

Vec3 Vec3::normalize() {
    float c = length();

    m_x = m_x / c;
    m_y = m_y / c;
    m_z = m_z / c;

    return *this;
}

float Vec3::length() {
    return sqrtf(powf(m_x, 2) + powf(m_y, 2) + powf(m_z, 2));
}