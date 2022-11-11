#ifndef __SR_VEC4F_H__
#define __SR_VEC4F_H__

#include "sr_vec3f.h"

//-----------------------------------------------------------------------------
// 3 维齐次向量或 4 维向量
//-----------------------------------------------------------------------------

/**
 * 4 维向量没有 3 维向量中点乘、叉乘等的方法。其主要用于 3 维仿射变换和齐次空间的裁剪，
 * 平常对于顶点或者模型的操作还是应以 SR_Vec3f 或者 SR_Vec2f 为主
 */
class SR_Vec4f
{
public:
    SR_Vec4f();
    SR_Vec4f(float xyz, float w);
    SR_Vec4f(const SR_Vec3f &v, float w = 1.0f);
    SR_Vec4f(float x, float y, float z, float w);
    SR_Vec4f(const SR_Vec4f &v);
    ~SR_Vec4f();

public:
    // p = (1 - t) * a + t * b 当 t = 0 时返回 a，否则返回 b
    static SR_Vec4f lerp(const SR_Vec4f &a, const SR_Vec4f &b, float t);

public:
    SR_Vec4f & operator=(const SR_Vec4f &v);

    // 向量加法和减法
    SR_Vec4f operator+(const SR_Vec4f &v) const;
    SR_Vec4f operator-(const SR_Vec4f &v) const;

    // 取反向量
    SR_Vec4f operator-() const;

    // 向量和实数相乘
    SR_Vec4f operator*(float t) const;
    friend SR_Vec4f operator*(float t, const SR_Vec4f &v);

    // 向量和实数相除
    SR_Vec4f operator/(float t) const;

    // 齐次除法
    void homogenDivide();

    // 打印向量
    void printValue(const char *title = nullptr) const;

public:
    float x;
    float y;
    float z;
    float w;
};

#endif // __SR_VEC4F_H__
