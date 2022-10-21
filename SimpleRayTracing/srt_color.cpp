#include <cstdio>
#include "srt_color.h"

//-----------------------------------------------------------------------------

SRT_Color::SRT_Color()
{
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
    a = 1.0f;
}

SRT_Color::SRT_Color(unsigned int rgba)
{
    *this = SRT_Color::fromUintRGB(rgba);
}

SRT_Color::SRT_Color(float rgb)
{
    r = rgb;
    g = rgb;
    b = rgb;
    a = 1.0;
}

SRT_Color::SRT_Color(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 1.0;
}

SRT_Color::SRT_Color(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

SRT_Color::SRT_Color(const SRT_Color &c)
{
    this->r = c.r;
    this->g = c.g;
    this->b = c.b;
    this->a = c.a;
}

SRT_Color::~SRT_Color()
{
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
    a = 1.0f;
}

SRT_Color SRT_Color::fromUintRGB(unsigned int rgb)
{
    SRT_Color color;

    int i = rgb & 0x000000FF;
    color.b = ((float)i) / 255.0f;

    i = (rgb & 0x0000FF00) >> 8;
    color.g = ((float)i) / 255.0f;

    i = (rgb & 0x00FF0000) >> 16;
    color.r = ((float)i) / 255.0f;

    return color;
}

SRT_Color & SRT_Color::operator=(const SRT_Color &c)
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

SRT_Color SRT_Color::operator+(const SRT_Color &c) const
{
    SRT_Color tmp = *this;
    tmp.r += c.r;
    tmp.g += c.g;
    tmp.b += c.b;
    return tmp;
}

SRT_Color SRT_Color::operator-(const SRT_Color &c) const
{
    SRT_Color tmp = *this;
    tmp.r -= c.r;
    tmp.g -= c.g;
    tmp.b -= c.b;
    return tmp;
}

SRT_Color SRT_Color::operator*(float t) const
{
    SRT_Color tmp = *this;
    tmp.r *= t;
    tmp.g *= t;
    tmp.b *= t;
    return tmp;
}

SRT_Color SRT_Color::operator*(const SRT_Color &c) const
{
    SRT_Color tmp = *this;
    tmp.r *= c.r;
    tmp.g *= c.g;
    tmp.b *= c.b;
    return tmp;
}

SRT_Color operator*(float t, const SRT_Color &c)
{
    SRT_Color tmp = c;
    tmp.r *= t;
    tmp.g *= t;
    tmp.b *= t;
    return tmp;
}

unsigned int SRT_Color::toUintRGB() const
{
    unsigned int ir = (unsigned int)(r * 255.0f);
    unsigned int ig = (unsigned int)(g * 255.0f);
    unsigned int ib = (unsigned int)(b * 255.0f);
    return (ir << 16) | (ig << 8) | ib;
}

void SRT_Color::setRGB(float rgb)
{
    r = rgb;
    g = rgb;
    b = rgb;
}

void SRT_Color::setRGB(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

void SRT_Color::printValue(const char *title) const
{
    if (!title || !title[0]) {
        title = "";
    }
    std::printf("%s:[%f, %f, %f, %f]\n", title, r, g, b, a);
}

//-----------------------------------------------------------------------------
