#include <cmath>
#include <cstdio>
#include "srt_vec2f.h"

//-----------------------------------------------------------------------------

SRT_Vec2f::SRT_Vec2f()
{
    x = 0.0f;
    y = 0.0f;
}

SRT_Vec2f::SRT_Vec2f(float xy)
{
    x = xy;
    y = xy;
}

SRT_Vec2f::SRT_Vec2f(float x, float y)
{
    this->x = x;
    this->y = y;
}

SRT_Vec2f::SRT_Vec2f(const SRT_Vec2f &v)
{
    x = v.x;
    y = v.y;
}

SRT_Vec2f::~SRT_Vec2f()
{
    x = 0.0f;
    y = 0.0f;
}

//---------------------------------------------------------
// 静态方法的实现

float SRT_Vec2f::dot(const SRT_Vec2f &v1, const SRT_Vec2f &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float SRT_Vec2f::angle(const SRT_Vec2f &v1, const SRT_Vec2f &v2)
{
    float len = v1.length() * v2.length();

    if (std::abs(len) < 1e-6) {
        return 0.0f;
    }

    float tmp = v1.dot(v2) / (len);
    return std::acos(tmp);
}

float SRT_Vec2f::crossValue(const SRT_Vec2f &v1, const SRT_Vec2f &v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

SRT_Vec2f SRT_Vec2f::normalize(const SRT_Vec2f &v)
{
    float len = v.length();
    return std::abs(len) > 1e-6 ? (v / len) : SRT_Vec2f(0.0f);
}

//---------------------------------------------------------

SRT_Vec2f & SRT_Vec2f::operator=(const SRT_Vec2f &v)
{
    if (&v == this) {
        return *this;
    }

    x = v.x;
    y = v.y;

    return *this;
}

SRT_Vec2f SRT_Vec2f::operator+(const SRT_Vec2f &v) const
{
    SRT_Vec2f vec = *this;
    vec.x += v.x;
    vec.y += v.y;
    return vec;
}

SRT_Vec2f SRT_Vec2f::operator-(const SRT_Vec2f &v) const
{
    SRT_Vec2f vec = *this;
    vec.x -= v.x;
    vec.y -= v.y;
    return vec;
}

SRT_Vec2f SRT_Vec2f::operator-() const
{
    SRT_Vec2f vec = *this;
    vec.x = -vec.x;
    vec.y = -vec.y;
    return vec;
}

SRT_Vec2f SRT_Vec2f::operator*(float t) const
{
    SRT_Vec2f vec = *this;
    vec.x *= t;
    vec.y *= t;
    return vec;
}

SRT_Vec2f operator*(float t, const SRT_Vec2f &v)
{
    SRT_Vec2f vec = v;
    vec.x *= t;
    vec.y *= t;
    return vec;
}

SRT_Vec2f SRT_Vec2f::operator/(float t) const
{
    if (std::abs(t) < 1e-6) {
        return SRT_Vec2f();
    }

    SRT_Vec2f vec = *this;
    vec.x /= t;
    vec.y /= t;
    return vec;
}

float SRT_Vec2f::length() const
{
    float val = x * x + y * y;
    return std::sqrt(val);
}

float SRT_Vec2f::dot(const SRT_Vec2f &v) const
{
    return SRT_Vec2f::dot(*this, v);
}

float SRT_Vec2f::angle(const SRT_Vec2f &v) const
{
    return SRT_Vec2f::angle(*this, v);
}

float SRT_Vec2f::crossValue(const SRT_Vec2f &v) const
{
    return SRT_Vec2f::crossValue(*this, v);
}

void SRT_Vec2f::normalize()
{
    *this = SRT_Vec2f::normalize(*this);
}

void SRT_Vec2f::printValue(const char *title) const
{
    if (!title || !title[0]) {
        title = "";
    }
    std::printf("%s:[%f, %f]\n", title, x, y);
}

//-----------------------------------------------------------------------------
