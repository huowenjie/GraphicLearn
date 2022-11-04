#include <cmath>
#include <cstdio>
#include "sr_vec2f.h"

//-----------------------------------------------------------------------------

SR_Vec2f::SR_Vec2f()
{
    this->x = 0.0f;
    this->y = 0.0f;
}

SR_Vec2f::SR_Vec2f(float xy)
{
    this->x = xy;
    this->y = xy;
}

SR_Vec2f::SR_Vec2f(float x, float y)
{
    this->x = x;
    this->y = y;
}

SR_Vec2f::SR_Vec2f(const SR_Vec2f &v)
{
    this->x = v.x;
    this->y = v.y;
}

SR_Vec2f::~SR_Vec2f()
{
}

//---------------------------------------------------------
// 静态方法的实现

float SR_Vec2f::dot(const SR_Vec2f &v1, const SR_Vec2f &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float SR_Vec2f::angle(const SR_Vec2f &v1, const SR_Vec2f &v2)
{
    float len = v1.length() * v2.length();

    if (std::abs(len) < 1e-6) {
        return 0.0f;
    }

    float tmp = v1.dot(v2) / (len);
    return std::acos(tmp);
}

float SR_Vec2f::crossValue(const SR_Vec2f &v1, const SR_Vec2f &v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

SR_Vec2f SR_Vec2f::normalize(const SR_Vec2f &v)
{
    float len = v.length();
    return std::abs(len) > 1e-6 ? (v / len) : SR_Vec2f(0.0f);
}

//---------------------------------------------------------

SR_Vec2f & SR_Vec2f::operator=(const SR_Vec2f &v)
{
    if (&v == this) {
        return *this;
    }

    this->x = v.x;
    this->y = v.y;

    return *this;
}

SR_Vec2f SR_Vec2f::operator+(const SR_Vec2f &v) const
{
    SR_Vec2f vec = *this;
    vec.x += v.x;
    vec.y += v.y;
    return vec;
}

SR_Vec2f SR_Vec2f::operator-(const SR_Vec2f &v) const
{
    SR_Vec2f vec = *this;
    vec.x -= v.x;
    vec.y -= v.y;
    return vec;
}

SR_Vec2f SR_Vec2f::operator-() const
{
    SR_Vec2f vec = *this;
    vec.x = -vec.x;
    vec.y = -vec.y;
    return vec;
}

SR_Vec2f SR_Vec2f::operator*(float t) const
{
    SR_Vec2f vec = *this;
    vec.x *= t;
    vec.y *= t;
    return vec;
}

SR_Vec2f operator*(float t, const SR_Vec2f &v)
{
    SR_Vec2f vec = v;
    vec.x *= t;
    vec.y *= t;
    return vec;
}

SR_Vec2f SR_Vec2f::operator/(float t) const
{
    if (std::abs(t) < 1e-6) {
        return SR_Vec2f();
    }

    SR_Vec2f vec = *this;
    vec.x /= t;
    vec.y /= t;
    return vec;
}

float SR_Vec2f::length() const
{
    float val = x * x + y * y;
    return std::sqrt(val);
}

float SR_Vec2f::dot(const SR_Vec2f &v) const
{
    return SR_Vec2f::dot(*this, v);
}

float SR_Vec2f::angle(const SR_Vec2f &v) const
{
    return SR_Vec2f::angle(*this, v);
}

float SR_Vec2f::crossValue(const SR_Vec2f &v) const
{
    return SR_Vec2f::crossValue(*this, v);
}

void SR_Vec2f::normalize()
{
    *this = SR_Vec2f::normalize(*this);
}

void SR_Vec2f::printValue(const char *title) const
{
    if (!title || !title[0]) {
        title = "";
    }
    std::printf("%s:[%f, %f]\n", title, x, y);
}

//-----------------------------------------------------------------------------
