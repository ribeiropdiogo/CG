#include "Vec3.h"
#include <math.h>

Vec3::Vec3() : Vec3(0.0f, 0.0f, 0.0f) {}

Vec3::Vec3(float x, float y, float z) {
    m_x = x;
    m_y = y;
    m_z = z;
}

Vec3::Vec3(const Vec3 &v) {
    m_x = v.m_x;
    m_y = v.m_y;
    m_z = v.m_z;
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

float Vec3::length() {

    return((float)sqrt(m_x*m_x + m_y*m_y + m_z*m_z));
}

void Vec3::normalize() {

    float len;

    len = length();
    if (len) {
        m_x /= len;;
        m_y /= len;
        m_z /= len;
    }
}

void Vec3::copy(const Vec3 &v) {

    m_x = v.m_x;
    m_y = v.m_y;
    m_z = v.m_z;
}

float Vec3::innerProduct(Vec3 &v) {

    return (m_x * v.getX() + m_y * v.getY() + m_z * v.getZ());
}

void Vec3::set(float x,float y, float z) {

    this->m_x = x;
    this->m_y = y;
    this->m_z = z;
}