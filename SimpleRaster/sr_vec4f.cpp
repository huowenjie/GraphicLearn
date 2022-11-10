#include <cmath>
#include <cstdio>
#include "sr_vec4f.h"

//-----------------------------------------------------------------------------

SR_Vec4f::SR_Vec4f()
{
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
    this->w = 0.0f;
}

SR_Vec4f::SR_Vec4f(float xyz, float w)
{
    this->x = xyz;
    this->y = xyz;
    this->z = xyz;
    this->w = w;
}

SR_Vec4f::SR_Vec4f(const SR_Vec3f &v, float w)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = w;
}

SR_Vec4f::SR_Vec4f(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

SR_Vec4f::SR_Vec4f(const SR_Vec4f &v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = v.w;
}

SR_Vec4f::~SR_Vec4f()
{
}

SR_Vec4f SR_Vec4f::lerp(const SR_Vec4f &a, const SR_Vec4f &b, float t)
{
    return a * (1.0f - t) + b * t;
}

SR_Vec4f & SR_Vec4f::operator=(const SR_Vec4f &v)
{
    if (&v == this) {
        return *this;
    }

    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = v.w;

    return *this;
}

SR_Vec4f SR_Vec4f::operator+(const SR_Vec4f &v) const
{
    SR_Vec4f vec = *this;
    vec.x += v.x;
    vec.y += v.y;
    vec.z += v.z;
    vec.w += v.w;
    return vec;
}

SR_Vec4f SR_Vec4f::operator-(const SR_Vec4f &v) const
{
    SR_Vec4f vec = *this;
    vec.x -= v.x;
    vec.y -= v.y;
    vec.z -= v.z;
    vec.w -= v.w;
    return vec;
}

SR_Vec4f SR_Vec4f::operator-() const
{
    SR_Vec4f vec = *this;
    vec.x = -vec.x;
    vec.y = -vec.y;
    vec.z = -vec.z;
    vec.w = -vec.w;
    return vec;
}

SR_Vec4f SR_Vec4f::operator*(float t) const
{
    SR_Vec4f vec = *this;
    vec.x *= t;
    vec.y *= t;
    vec.z *= t;
    vec.w *= t;
    return vec;
}

SR_Vec4f operator*(float t, const SR_Vec4f &v)
{
    SR_Vec4f vec = v;
    vec.x *= t;
    vec.y *= t;
    vec.z *= t;
    vec.w *= t;
    return vec;
}

SR_Vec4f SR_Vec4f::operator/(float t) const
{
    if (std::abs(t) < 1e-6) {
        return SR_Vec4f();
    }

    SR_Vec4f vec(*this);
    vec.x /= t;
    vec.y /= t;
    vec.z /= t;
    vec.w /= t;
    return vec;
}

void SR_Vec4f::homogenDivide()
{
    if (std::abs(w) < 1e-6) {
        return;
    }

    x /= w;
    y /= w;
    z /= w;
    w = 1.0f;
}

void SR_Vec4f::printValue(const char *title) const
{
    if (!title || !title[0]) {
        title = "";
    }
    std::printf("%s:[%f, %f, %f, %f]\n", title, x, y, z, w);
}

//-----------------------------------------------------------------------------
