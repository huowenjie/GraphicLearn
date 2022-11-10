#include <cstdio>
#include "sr_color.h"
#include "sr_global.h"

//-----------------------------------------------------------------------------

SR_Color::SR_Color()
{
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
    a = 1.0f;
}

SR_Color::SR_Color(unsigned int rgba)
{
    *this = SR_Color::fromUintRGB(rgba);
}

SR_Color::SR_Color(float rgb)
{
    r = rgb;
    g = rgb;
    b = rgb;
    a = 1.0;
}

SR_Color::SR_Color(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 1.0;
}

SR_Color::SR_Color(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

SR_Color::SR_Color(const SR_Color &c)
{
    this->r = c.r;
    this->g = c.g;
    this->b = c.b;
    this->a = c.a;
}

SR_Color::~SR_Color()
{
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
    a = 1.0f;
}

SR_Color SR_Color::fromUintRGB(unsigned int rgb)
{
    SR_Color color;

    int i = rgb & 0x000000FF;
    color.b = ((float)i) / 255.0f;

    i = (rgb & 0x0000FF00) >> 8;
    color.g = ((float)i) / 255.0f;

    i = (rgb & 0x00FF0000) >> 16;
    color.r = ((float)i) / 255.0f;

    return color;
}

SR_Color SR_Color::lerp(const SR_Color &a, const SR_Color &b, float t)
{
    return a * (1.0f - t) + b * t;
}

SR_Color & SR_Color::operator=(const SR_Color &c)
{
    if (&c == this) {
        return *this;
    }

    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;

    return *this;
}

SR_Color SR_Color::operator+(const SR_Color &c) const
{
    SR_Color tmp = *this;
    tmp.r += c.r;
    tmp.g += c.g;
    tmp.b += c.b;
    return tmp;
}

SR_Color SR_Color::operator-(const SR_Color &c) const
{
    SR_Color tmp = *this;
    tmp.r -= c.r;
    tmp.g -= c.g;
    tmp.b -= c.b;
    return tmp;
}

SR_Color SR_Color::operator*(float t) const
{
    SR_Color tmp = *this;
    tmp.r *= t;
    tmp.g *= t;
    tmp.b *= t;
    return tmp;
}

SR_Color SR_Color::operator*(const SR_Color &c) const
{
    SR_Color tmp = *this;
    tmp.r *= c.r;
    tmp.g *= c.g;
    tmp.b *= c.b;
    return tmp;
}

SR_Color operator*(float t, const SR_Color &c)
{
    SR_Color tmp = c;
    tmp.r *= t;
    tmp.g *= t;
    tmp.b *= t;
    return tmp;
}

unsigned int SR_Color::toUintRGB() const
{
    unsigned int ir = (unsigned int)(r * 255.0f);
    unsigned int ig = (unsigned int)(g * 255.0f);
    unsigned int ib = (unsigned int)(b * 255.0f);
    return (ir << 16) | (ig << 8) | ib;
}

void SR_Color::setRGB(float rgb)
{
    r = rgb;
    g = rgb;
    b = rgb;
}

void SR_Color::setRGB(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

void SR_Color::clamp()
{
    this->r = srtClampf(r, 1.0f, 0.0f);
    this->g = srtClampf(g, 1.0f, 0.0f);
    this->b = srtClampf(b, 1.0f, 0.0f);
    this->a = srtClampf(a, 1.0f, 0.0f);
}

void SR_Color::printValue(const char *title) const
{
    if (!title || !title[0]) {
        title = "";
    }
    std::printf("%s:[%f, %f, %f, %f]\n", title, r, g, b, a);
}

//-----------------------------------------------------------------------------
