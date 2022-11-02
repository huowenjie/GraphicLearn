#include <cmath>
#include <cstdio>
#include "sr_mat3x3f.h"

//-----------------------------------------------------------------------------

SR_Mat3x3f::SR_Mat3x3f()
{
    this->m00 = this->m11 = this->m22 = 1.0f;
    this->m01 = this->m02 = 0.0f;
    this->m10 = this->m12 = 0.0f;
    this->m20 = this->m21 = 0.0f;
}

SR_Mat3x3f::SR_Mat3x3f(float elem)
{
    this->m00 = this->m01 = this->m02 = elem;
    this->m10 = this->m11 = this->m12 = elem;
    this->m20 = this->m21 = this->m22 = elem;
}

SR_Mat3x3f::SR_Mat3x3f(float *elems)
{
    this->m00 = elems[0];
    this->m01 = elems[1];
    this->m02 = elems[2];
    this->m10 = elems[3];
    this->m11 = elems[4];
    this->m12 = elems[5];
    this->m20 = elems[6];
    this->m21 = elems[7];
    this->m22 = elems[8];
}

SR_Mat3x3f::SR_Mat3x3f(float (*elems)[3])
{
    this->m00 = elems[0][0];
    this->m01 = elems[0][1];
    this->m02 = elems[0][2];
    this->m10 = elems[1][0];
    this->m11 = elems[1][1];
    this->m12 = elems[1][2];
    this->m20 = elems[2][0];
    this->m21 = elems[2][1];
    this->m22 = elems[2][2];
}

SR_Mat3x3f::SR_Mat3x3f(const SR_Mat3x3f &mat)
{
    this->m00 = mat.m00;
    this->m01 = mat.m01;
    this->m02 = mat.m02;
    this->m10 = mat.m10;
    this->m11 = mat.m11;
    this->m12 = mat.m12;
    this->m20 = mat.m20;
    this->m21 = mat.m21;
    this->m22 = mat.m22;
}

SR_Mat3x3f::~SR_Mat3x3f()
{
    this->m00 = 0.0f;
    this->m01 = 0.0f;
    this->m02 = 0.0f;
    this->m10 = 0.0f;
    this->m11 = 0.0f;
    this->m12 = 0.0f;
    this->m20 = 0.0f;
    this->m21 = 0.0f;
    this->m22 = 0.0f;
}

SR_Mat3x3f SR_Mat3x3f::unitMatrix()
{
    return SR_Mat3x3f();
}

SR_Mat3x3f SR_Mat3x3f::zeroMatrix()
{
    return SR_Mat3x3f(0.0f);
}

SR_Mat3x3f SR_Mat3x3f::transpose(const SR_Mat3x3f &mat)
{
    SR_Mat3x3f tmp(mat);

    tmp.m01 = mat.m10;
    tmp.m10 = mat.m01;
    tmp.m02 = mat.m20;
    tmp.m20 = mat.m02;
    tmp.m12 = mat.m21;
    tmp.m21 = mat.m12;

    return tmp;
}

SR_Mat3x3f SR_Mat3x3f::inverse(const SR_Mat3x3f &mat)
{
    return SR_Mat3x3f();
}

SR_Mat3x3f & SR_Mat3x3f::operator=(const SR_Mat3x3f &mat)
{
    if (&mat == this) {
        return *this;
    }

    this->m00 = mat.m00;
    this->m01 = mat.m01;
    this->m02 = mat.m02;
    this->m10 = mat.m10;
    this->m11 = mat.m11;
    this->m12 = mat.m12;
    this->m20 = mat.m20;
    this->m21 = mat.m21;
    this->m22 = mat.m22;
    return *this;
}

SR_Mat3x3f SR_Mat3x3f::operator+(const SR_Mat3x3f &mat) const
{
    return SR_Mat3x3f();
}

SR_Mat3x3f SR_Mat3x3f::operator-(const SR_Mat3x3f &mat) const
{
    return SR_Mat3x3f();
}

SR_Mat3x3f SR_Mat3x3f::operator*(float val) const
{
    return SR_Mat3x3f();
}


SR_Mat3x3f operator*(float val, const SR_Mat3x3f &mat)
{
    return SR_Mat3x3f();
}

// 矩阵相乘
SR_Mat3x3f SR_Mat3x3f::operator*(const SR_Mat3x3f &mat) const
{
    return SR_Mat3x3f();
}

SR_Vec3f SR_Mat3x3f::operator*(const SR_Vec3f &vec) const
{
    return SR_Vec3f();
}

// 矩阵取值
float *SR_Mat3x3f::operator[](int i)
{
    return NULL;
}

SR_Mat3x3f SR_Mat3x3f::operator-() const
{
    return SR_Mat3x3f();
}

SR_Mat3x3f SR_Mat3x3f::transpose() const
{
    return SR_Mat3x3f();
}

void SR_Mat3x3f::printValue(const char *title) const
{

}

//-----------------------------------------------------------------------------
