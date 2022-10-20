#include <cmath>
#include <cstdio>
#include "srt_vec2f.h"

//-----------------------------------------------------------------------------

SRT_Vec2f::SRT_Vec2f()
{
    vecX = 0.0f;
    vecY = 0.0f;
}

SRT_Vec2f::SRT_Vec2f(float xy)
{
    vecX = xy;
    vecY = xy;
}

SRT_Vec2f::SRT_Vec2f(float x, float y)
{
    vecX = x;
    vecY = y;
}

SRT_Vec2f::SRT_Vec2f(const SRT_Vec2f &v)
{
    vecX = v.vecX;
    vecY = v.vecY;
}

SRT_Vec2f::~SRT_Vec2f()
{
    vecX = 0.0f;
    vecY = 0.0f;
}

SRT_Vec2f & SRT_Vec2f::operator=(const SRT_Vec2f &v)
{
    if (&v == this) {
        return *this;
    }

    vecX = v.vecX;
    vecY = v.vecY;

    return *this;
}

SRT_Vec2f SRT_Vec2f::operator+(const SRT_Vec2f &v) const
{
    SRT_Vec2f vec = *this;
    vec.vecX += v.vecX;
    vec.vecY += v.vecY;
    return vec;
}

SRT_Vec2f SRT_Vec2f::operator-(const SRT_Vec2f &v) const
{
    SRT_Vec2f vec = *this;
    vec.vecX -= v.vecX;
    vec.vecY -= v.vecY;
    return vec;
}

SRT_Vec2f SRT_Vec2f::operator-() const
{
    SRT_Vec2f vec = *this;
    vec.vecX = -vec.vecX;
    vec.vecY = -vec.vecY;
    return vec;
}

SRT_Vec2f SRT_Vec2f::operator*(float t) const
{
    SRT_Vec2f vec = *this;
    vec.vecX *= t;
    vec.vecY *= t;
    return vec;
}

SRT_Vec2f operator*(float t, const SRT_Vec2f &v)
{
    SRT_Vec2f vec = v;
    vec.vecX *= t;
    vec.vecY *= t;
    return vec;
}

float SRT_Vec2f::length() const
{
    float val = vecX * vecX + vecY * vecY;
    return (float)std::sqrt(val);
}

void SRT_Vec2f::printValue(const char *title)
{
    if (!title || !title[0]) {
        title = "";
    }
    std::printf("%s:[%f, %f]\n", title, vecX, vecY);
}

//-----------------------------------------------------------------------------
