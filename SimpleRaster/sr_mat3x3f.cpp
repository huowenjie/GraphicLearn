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
    // 求行列式
    float det = 
        mat.m00 * mat.m11 * mat.m22 +
        mat.m02 * mat.m10 * mat.m21 +
        mat.m01 * mat.m12 * mat.m20 -
        mat.m00 * mat.m21 * mat.m12 - 
        mat.m01 * mat.m10 * mat.m22 -
        mat.m02 * mat.m11 * mat.m20;

    // 行列式为 0，不可逆，直接返回原矩阵
    if (std::abs(det) < 1e-6f) {
        return mat;
    }

    /*
     * 利用克拉默法则来求解逆矩阵
     * inverse A = adj A / det A
     */
    SR_Mat3x3f adj;

    adj.m00 = (mat.m11 * mat.m22 - mat.m12 * mat.m21) / det;
    adj.m01 = (mat.m02 * mat.m21 - mat.m01 * mat.m22) / det;
    adj.m02 = (mat.m01 * mat.m12 - mat.m02 * mat.m11) / det;

    adj.m10 = (mat.m12 * mat.m20 - mat.m10 * mat.m22) / det;
    adj.m11 = (mat.m00 * mat.m22 - mat.m02 * mat.m20) / det;
    adj.m12 = (mat.m02 * mat.m10 - mat.m00 * mat.m12) / det;

    adj.m20 = (mat.m10 * mat.m21 - mat.m11 * mat.m20) / det;
    adj.m21 = (mat.m01 * mat.m20 - mat.m00 * mat.m21) / det;
    adj.m22 = (mat.m00 * mat.m11 - mat.m01 * mat.m10) / det;

    return adj;
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
    SR_Mat3x3f tmp(*this);

    tmp.m00 += mat.m00;
    tmp.m01 += mat.m01;
    tmp.m02 += mat.m02;
    tmp.m10 += mat.m10;
    tmp.m11 += mat.m11;
    tmp.m12 += mat.m12;
    tmp.m20 += mat.m20;
    tmp.m21 += mat.m21;
    tmp.m22 += mat.m22;

    return tmp;
}

SR_Mat3x3f SR_Mat3x3f::operator-(const SR_Mat3x3f &mat) const
{
    SR_Mat3x3f tmp(*this);

    tmp.m00 -= mat.m00;
    tmp.m01 -= mat.m01;
    tmp.m02 -= mat.m02;
    tmp.m10 -= mat.m10;
    tmp.m11 -= mat.m11;
    tmp.m12 -= mat.m12;
    tmp.m20 -= mat.m20;
    tmp.m21 -= mat.m21;
    tmp.m22 -= mat.m22;

    return tmp;
}

SR_Mat3x3f SR_Mat3x3f::operator*(float val) const
{
    SR_Mat3x3f tmp(*this);

    tmp.m00 *= val;
    tmp.m01 *= val;
    tmp.m02 *= val;
    tmp.m10 *= val;
    tmp.m11 *= val;
    tmp.m12 *= val;
    tmp.m20 *= val;
    tmp.m21 *= val;
    tmp.m22 *= val;

    return tmp;
}

SR_Mat3x3f operator*(float val, const SR_Mat3x3f &mat)
{
    SR_Mat3x3f tmp(mat);

    tmp.m00 *= val;
    tmp.m01 *= val;
    tmp.m02 *= val;
    tmp.m10 *= val;
    tmp.m11 *= val;
    tmp.m12 *= val;
    tmp.m20 *= val;
    tmp.m21 *= val;
    tmp.m22 *= val;

    return tmp;
}

SR_Mat3x3f SR_Mat3x3f::operator*(const SR_Mat3x3f &mat) const
{
    SR_Mat3x3f tmp(0.0f);

    tmp.m00 = m00 * mat.m00 + m01 * mat.m10 + m02 * mat.m20;
    tmp.m01 = m00 * mat.m01 + m01 * mat.m11 + m02 * mat.m21;
    tmp.m02 = m00 * mat.m02 + m01 * mat.m12 + m02 * mat.m22;
    tmp.m10 = m10 * mat.m00 + m11 * mat.m10 + m12 * mat.m20;
    tmp.m11 = m10 * mat.m01 + m11 * mat.m11 + m12 * mat.m21;
    tmp.m12 = m10 * mat.m02 + m11 * mat.m12 + m12 * mat.m22;
    tmp.m20 = m20 * mat.m00 + m21 * mat.m10 + m22 * mat.m20;
    tmp.m21 = m20 * mat.m01 + m21 * mat.m11 + m22 * mat.m21;
    tmp.m22 = m20 * mat.m02 + m21 * mat.m12 + m22 * mat.m22;

    return tmp;
}

SR_Vec3f SR_Mat3x3f::operator*(const SR_Vec3f &vec) const
{
    SR_Vec3f tmp;

    tmp.x = m00 * vec.x + m01 * vec.y + m02 * vec.z;
    tmp.y = m10 * vec.x + m11 * vec.y + m12 * vec.z;
    tmp.z = m20 * vec.x + m21 * vec.y + m22 * vec.z;

    return tmp;
}

float *SR_Mat3x3f::operator[](int i)
{
    return matij[i];
}

SR_Mat3x3f SR_Mat3x3f::operator-() const
{
    return SR_Mat3x3f::inverse(*this);
}

SR_Mat3x3f SR_Mat3x3f::transpose() const
{
    return SR_Mat3x3f::transpose(*this);
}

void SR_Mat3x3f::printValue(const char *title) const
{
    if (!title || !title[0]) {
        title = "";
    }

    std::printf(
        "%s:\n"
        "[%f, %f, %f]\n"
        "[%f, %f, %f]\n"
        "[%f, %f, %f]\n",
        title,
        m00, m01, m02,
        m10, m11, m12,
        m20, m21, m22
    );
}

//-----------------------------------------------------------------------------
