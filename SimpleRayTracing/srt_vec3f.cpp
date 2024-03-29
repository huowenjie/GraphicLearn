#include <cmath>
#include <cstdio>
#include "srt_vec3f.h"

//-----------------------------------------------------------------------------

SRT_Vec3f::SRT_Vec3f()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

SRT_Vec3f::SRT_Vec3f(float xyz)
{
    x = xyz;
    y = xyz;
    z = xyz;
}

SRT_Vec3f::SRT_Vec3f(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

SRT_Vec3f::SRT_Vec3f(const SRT_Vec3f &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

SRT_Vec3f::~SRT_Vec3f()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

//---------------------------------------------------------
// 静态方法的实现

float SRT_Vec3f::dot(const SRT_Vec3f &v1, const SRT_Vec3f &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

SRT_Vec3f SRT_Vec3f::cross(const SRT_Vec3f &v1, const SRT_Vec3f &v2)
{
    return SRT_Vec3f(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

float SRT_Vec3f::crossValue(const SRT_Vec3f &v1, const SRT_Vec3f &v2)
{
    SRT_Vec3f vp = SRT_Vec3f::cross(v1, v2);
    return vp.length();
}

float SRT_Vec3f::angle(const SRT_Vec3f &v1, const SRT_Vec3f &v2)
{
    float len = v1.length() * v2.length();

    if (std::abs(len) < 1e-6) {
        return 0.0f;
    }

    float tmp = v1.dot(v2) / (len);
    return std::acos(tmp);
}

SRT_Vec3f SRT_Vec3f::normalize(const SRT_Vec3f &v)
{
    float len = v.length();
    return std::abs(len) > 1e-6 ? (v / len) : SRT_Vec3f(0.0f);
}

//---------------------------------------------------------

SRT_Vec3f & SRT_Vec3f::operator=(const SRT_Vec3f &v)
{
    if (&v == this) {
        return *this;
    }

    x = v.x;
    y = v.y;
    z = v.z;

    return *this;
}

SRT_Vec3f SRT_Vec3f::operator+(const SRT_Vec3f &v) const
{
    SRT_Vec3f vec = *this;
    vec.x += v.x;
    vec.y += v.y;
    vec.z += v.z;
    return vec;
}

SRT_Vec3f SRT_Vec3f::operator-(const SRT_Vec3f &v) const
{
    SRT_Vec3f vec = *this;
    vec.x -= v.x;
    vec.y -= v.y;
    vec.z -= v.z;
    return vec;
}

SRT_Vec3f SRT_Vec3f::operator-() const
{
    SRT_Vec3f vec = *this;
    vec.x = -vec.x;
    vec.y = -vec.y;
    vec.z = -vec.z;
    return vec;
}

SRT_Vec3f SRT_Vec3f::operator*(float t) const
{
    SRT_Vec3f vec = *this;
    vec.x *= t;
    vec.y *= t;
    vec.z *= t;
    return vec;
}

SRT_Vec3f operator*(float t, const SRT_Vec3f &v)
{
    SRT_Vec3f vec = v;
    vec.x *= t;
    vec.y *= t;
    vec.z *= t;
    return vec;
}

SRT_Vec3f SRT_Vec3f::operator/(float t) const
{
    if (std::abs(t) < 1e-6) {
        return SRT_Vec3f();
    }

    SRT_Vec3f vec = *this;
    vec.x /= t;
    vec.y /= t;
    vec.z /= t;
    return vec;
}

float SRT_Vec3f::length() const
{
    float val = x * x + y * y + z * z;
    return std::sqrt(val);
}

float SRT_Vec3f::dot(const SRT_Vec3f &v) const
{
    return SRT_Vec3f::dot(*this, v);
}

float SRT_Vec3f::angle(const SRT_Vec3f &v) const
{
    return SRT_Vec3f::angle(*this, v);
}

float SRT_Vec3f::crossValue(const SRT_Vec3f &v) const
{
    return SRT_Vec3f::crossValue(*this, v);
}

SRT_Vec3f SRT_Vec3f::cross(const SRT_Vec3f &v) const
{
    return SRT_Vec3f::cross(*this, v);
}

void SRT_Vec3f::normalize()
{
    *this = SRT_Vec3f::normalize(*this);
}

void SRT_Vec3f::printValue(const char *title) const
{
    if (!title || !title[0]) {
        title = "";
    }
    std::printf("%s:[%f, %f, %f]\n", title, x, y, z);
}

//-----------------------------------------------------------------------------
