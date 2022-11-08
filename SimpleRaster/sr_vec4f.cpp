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

float SR_Vec4f::dot(const SR_Vec4f &v1, const SR_Vec4f &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

SR_Vec4f SR_Vec4f::cross(const SR_Vec4f &v1, const SR_Vec4f &v2)
{
    return SR_Vec4f(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
        0.0f
    );
}

float SR_Vec4f::crossValue(const SR_Vec4f &v1, const SR_Vec4f &v2)
{
    SR_Vec4f vp = SR_Vec4f::cross(v1, v2);
    return vp.length();
}

float SR_Vec4f::angle(const SR_Vec4f &v1, const SR_Vec4f &v2)
{
    float len = v1.length() * v2.length();

    if (std::abs(len) < 1e-6) {
        return 0.0f;
    }

    float tmp = v1.dot(v2) / (len);
    return std::acos(tmp);
}

SR_Vec4f SR_Vec4f::normalize(const SR_Vec4f &v)
{
    float len = v.length();

    if (std::abs(len) < 1e-6) {
        return SR_Vec4f();
    }

    return SR_Vec4f(
        v.x / len, v.y / len, v.z / len, 0.0f
    );
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
    return vec;
}

SR_Vec4f SR_Vec4f::operator-(const SR_Vec4f &v) const
{
    SR_Vec4f vec = *this;
    vec.x -= v.x;
    vec.y -= v.y;
    vec.z -= v.z;
    return vec;
}

SR_Vec4f SR_Vec4f::operator-() const
{
    SR_Vec4f vec = *this;
    vec.x = -vec.x;
    vec.y = -vec.y;
    vec.z = -vec.z;
    return vec;
}

SR_Vec4f SR_Vec4f::operator*(float t) const
{
    SR_Vec4f vec = *this;
    vec.x *= t;
    vec.y *= t;
    vec.z *= t;
    return vec;
}

SR_Vec4f operator*(float t, const SR_Vec4f &v)
{
    SR_Vec4f vec = v;
    vec.x *= t;
    vec.y *= t;
    vec.z *= t;
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
    return vec;
}

float SR_Vec4f::length() const
{
    float val = x * x + y * y + z * z;
    return std::sqrt(val);
}

float SR_Vec4f::dot(const SR_Vec4f &v) const
{
    return SR_Vec4f::dot(*this, v);
}

float SR_Vec4f::angle(const SR_Vec4f &v) const
{
    return SR_Vec4f::angle(*this, v);
}

float SR_Vec4f::crossValue(const SR_Vec4f &v) const
{
    return SR_Vec4f::crossValue(*this, v);
}

SR_Vec4f SR_Vec4f::cross(const SR_Vec4f &v) const
{
    return SR_Vec4f::cross(*this, v);
}

void SR_Vec4f::normalize()
{
    *this = SR_Vec4f::normalize(*this);
}

void SR_Vec4f::homogeneous()
{
    if (std::abs(w) > 1e-6) {
		x /= w;
        y /= w;
        z /= w;
        w = 1.0f;
	}
}

void SR_Vec4f::printValue(const char *title) const
{
    if (!title || !title[0]) {
        title = "";
    }
    std::printf("%s:[%f, %f, %f, %f]\n", title, x, y, z, w);
}

//-----------------------------------------------------------------------------
