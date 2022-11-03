#include <cmath>
#include <cstdio>
#include "sr_mat2x2f.h"

//-----------------------------------------------------------------------------

SR_Mat2x2f::SR_Mat2x2f()
{
    this->m00 = 1.0f;
    this->m01 = 0.0f;
    this->m10 = 0.0f;
    this->m11 = 1.0f;
}

SR_Mat2x2f::SR_Mat2x2f(float elem)
{
    this->m00 = elem;
    this->m01 = elem;
    this->m10 = elem;
    this->m11 = elem;
}

SR_Mat2x2f::SR_Mat2x2f(float *elems)
{
    this->m00 = elems[0];
    this->m01 = elems[1];
    this->m10 = elems[2];
    this->m11 = elems[3];
}

SR_Mat2x2f::SR_Mat2x2f(float (*elems)[2])
{
    this->m00 = elems[0][0];
    this->m01 = elems[0][1];
    this->m10 = elems[1][0];
    this->m11 = elems[1][1];
}

SR_Mat2x2f::SR_Mat2x2f(const SR_Mat2x2f &mat)
{
    this->m00 = mat.m00;
    this->m01 = mat.m01;
    this->m10 = mat.m10;
    this->m11 = mat.m11;
}

SR_Mat2x2f::~SR_Mat2x2f()
{
    this->m00 = 0.0f;
    this->m01 = 0.0f;
    this->m10 = 0.0f;
    this->m11 = 0.0f;
}

SR_Mat2x2f SR_Mat2x2f::unitMatrix()
{
    return SR_Mat2x2f();
}

SR_Mat2x2f SR_Mat2x2f::zeroMatrix()
{
    return SR_Mat2x2f(0.0f);
}

SR_Mat2x2f SR_Mat2x2f::transpose(const SR_Mat2x2f &mat)
{
    SR_Mat2x2f tmp(mat);
    tmp.m01 = mat.m10;
    tmp.m10 = mat.m01;
    return tmp;
}

SR_Mat2x2f SR_Mat2x2f::inverse(const SR_Mat2x2f &mat)
{
    float det = mat.m00 * mat.m11 - mat.m01 * mat.m10;
    
    // 行列式为 0，不可逆，直接返回原矩阵
    if (std::abs(det) < 1e-6f) {
        return mat;
    }

    SR_Mat2x2f tmp;
    tmp.m00 = mat.m11;
    tmp.m01 = -mat.m01;
    tmp.m10 = -mat.m10;
    tmp.m11 = mat.m00;

    det = 1.0f / det;
    return tmp * det;
}

SR_Mat2x2f & SR_Mat2x2f::operator=(const SR_Mat2x2f &mat)
{
    if (&mat == this) {
        return *this;
    }

    this->m00 = mat.m00;
    this->m01 = mat.m01;
    this->m10 = mat.m10;
    this->m11 = mat.m11;
    return *this;
}

SR_Mat2x2f SR_Mat2x2f::operator+(const SR_Mat2x2f &mat) const
{
    SR_Mat2x2f tmp(*this);

    tmp.m00 += mat.m00;
    tmp.m01 += mat.m01;
    tmp.m10 += mat.m10;
    tmp.m11 += mat.m11;

    return tmp;
}

SR_Mat2x2f SR_Mat2x2f::operator-(const SR_Mat2x2f &mat) const
{
    SR_Mat2x2f tmp(*this);

    tmp.m00 -= mat.m00;
    tmp.m01 -= mat.m01;
    tmp.m10 -= mat.m10;
    tmp.m11 -= mat.m11;

    return tmp;
}

SR_Mat2x2f SR_Mat2x2f::operator*(float val) const
{
    SR_Mat2x2f tmp(*this);

    tmp.m00 *= val;
    tmp.m01 *= val;
    tmp.m10 *= val;
    tmp.m11 *= val;

    return tmp;
}

SR_Mat2x2f operator*(float val, const SR_Mat2x2f &mat)
{
    SR_Mat2x2f tmp(mat);

    tmp.m00 *= val;
    tmp.m01 *= val;
    tmp.m10 *= val;
    tmp.m11 *= val;

    return tmp;
}

SR_Mat2x2f SR_Mat2x2f::operator*(const SR_Mat2x2f &mat) const
{
    SR_Mat2x2f tmp(0.0f);

    tmp.m00 = m00 * mat.m00 + m01 * mat.m10;
    tmp.m01 = m00 * mat.m01 + m01 * mat.m11;
    tmp.m10 = m10 * mat.m00 + m11 * mat.m10;
    tmp.m11 = m10 * mat.m01 + m11 * mat.m11;

    return tmp;
}

SR_Vec2f SR_Mat2x2f::operator*(const SR_Vec2f &vec) const
{
    SR_Vec2f tmp;

    tmp.x = m00 * vec.x + m01 * vec.y;
    tmp.y = m10 * vec.x + m11 * vec.y;

    return tmp;
}

float *SR_Mat2x2f::operator[](int i)
{
    return matij[i];
}

SR_Mat2x2f SR_Mat2x2f::operator-() const
{
    return SR_Mat2x2f::inverse(*this);
}

SR_Mat2x2f SR_Mat2x2f::transpose() const
{
    return SR_Mat2x2f::transpose(*this);
}

void SR_Mat2x2f::printValue(const char *title) const
{
    if (!title || !title[0]) {
        title = "";
    }

    std::printf(
        "%s:\n"
        "[%f, %f]\n"
        "[%f, %f]\n",
        title, m00, m01, m10, m11
    );
}

//-----------------------------------------------------------------------------
