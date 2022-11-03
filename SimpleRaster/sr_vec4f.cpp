#include "sr_vec4f.h"

//-----------------------------------------------------------------------------

SR_Vec4f::SR_Vec4f()
{
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
    this->w = 0.0f;
}

SR_Vec4f::SR_Vec4f(float xyzw)
{
    this->x = xyzw;
    this->y = xyzw;
    this->z = xyzw;
    this->w = xyzw;
}

SR_Vec4f::SR_Vec4f(float xyz, float w)
{
    this->x = xyz;
    this->y = xyz;
    this->z = xyz;
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
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
    this->w = 0.0f;
}

float SR_Vec4f::dot(const SR_Vec4f &v1, const SR_Vec4f &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

SR_Vec4f SR_Vec4f::cross(const SR_Vec4f &v1, const SR_Vec4f &v2)
{
    return SR_Vec4f();
}

float SR_Vec4f::crossValue(const SR_Vec4f &v1, const SR_Vec4f &v2)
{
    return 0.0f;
}

float SR_Vec4f::angle(const SR_Vec4f &v1, const SR_Vec4f &v2)
{
    return 0.0f;
}

SR_Vec4f SR_Vec4f::normalize(const SR_Vec4f &v)
{
    return SR_Vec4f();
}

SR_Vec4f & SR_Vec4f::operator=(const SR_Vec4f &v)
{
    return *this;
}

SR_Vec4f SR_Vec4f::operator+(const SR_Vec4f &v) const
{
    return SR_Vec4f();
}

SR_Vec4f SR_Vec4f::operator-(const SR_Vec4f &v) const
{
    return SR_Vec4f();
}

SR_Vec4f SR_Vec4f::operator-() const
{
    return SR_Vec4f();
}

SR_Vec4f SR_Vec4f::operator*(float t) const
{
    return SR_Vec4f();
}

SR_Vec4f operator*(float t, const SR_Vec4f &v)
{
    return SR_Vec4f();
}

SR_Vec4f SR_Vec4f::operator/(float t) const
{
    return SR_Vec4f();
}

float SR_Vec4f::length() const
{
    return 0.0f;
}

float SR_Vec4f::dot(const SR_Vec4f &v) const
{
    return 0.0f;
}

float SR_Vec4f::angle(const SR_Vec4f &v) const
{
    return 0.0f;
}

float SR_Vec4f::crossValue(const SR_Vec4f &v) const
{
    return 0.0f;
}

SR_Vec4f SR_Vec4f::cross(const SR_Vec4f &v) const
{
    return SR_Vec4f();
}

void SR_Vec4f::normalize()
{

}

void SR_Vec4f::printValue(const char *title = nullptr) const
{

}

//-----------------------------------------------------------------------------
