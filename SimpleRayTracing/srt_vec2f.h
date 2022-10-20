#ifndef __SRT_VEC2F_H__
#define __SRT_VEC2F_H__

#include <cstddef>

//-----------------------------------------------------------------------------
// 2 维向量
//-----------------------------------------------------------------------------

class SRT_Vec2f
{
public:
    SRT_Vec2f();
    SRT_Vec2f(float xy);
    SRT_Vec2f(float x, float y);
    SRT_Vec2f(const SRT_Vec2f &v);
    ~SRT_Vec2f();

public:
    SRT_Vec2f & operator=(const SRT_Vec2f &v);

    // 向量加法和减法
    SRT_Vec2f operator+(const SRT_Vec2f &v) const;
    SRT_Vec2f operator-(const SRT_Vec2f &v) const;

    // 取反向量
    SRT_Vec2f operator-() const;

    // 向量和实数相乘
    SRT_Vec2f operator*(float t) const;
    friend SRT_Vec2f operator*(float t, const SRT_Vec2f &v);

    // 向量长度
    float length() const;

    // 打印向量
    void printValue(const char *title = NULL);

public:
    float vecX;
    float vecY;
};

//-----------------------------------------------------------------------------

#endif // __SRT_VEC2F_H__
