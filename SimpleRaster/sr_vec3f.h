#ifndef __SR_VEC3F_H__
#define __SR_VEC3F_H__

#include "sr_vec2f.h"

//-----------------------------------------------------------------------------
// 3 维向量
//-----------------------------------------------------------------------------

class SR_Vec3f
{
public:
    SR_Vec3f();
    SR_Vec3f(float xyz);
    SR_Vec3f(const SR_Vec2f &v, float z = 0.0f);
    SR_Vec3f(float x, float y, float z);
    SR_Vec3f(const SR_Vec3f &v);
    ~SR_Vec3f();

public:
    // 向量点乘
    static float dot(const SR_Vec3f &v1, const SR_Vec3f &v2);

    // 向量叉乘
    static SR_Vec3f cross(const SR_Vec3f &v1, const SR_Vec3f &v2);

    // 向量叉乘的模
    static float crossValue(const SR_Vec3f &v1, const SR_Vec3f &v2);

    // 求向量的夹角，弧度
    static float angle(const SR_Vec3f &v1, const SR_Vec3f &v2);

    // 向量归一化并返回一个新向量
    static SR_Vec3f normalize(const SR_Vec3f &v);

    // p = (1 - t) * a + t * b 当 t = 0 时返回 a，否则返回 b
    static SR_Vec3f lerp(const SR_Vec3f &a, const SR_Vec3f &b, float t);

public:
    SR_Vec3f & operator=(const SR_Vec3f &v);

    // 向量加法和减法
    SR_Vec3f operator+(const SR_Vec3f &v) const;
    SR_Vec3f operator-(const SR_Vec3f &v) const;

    // 取反向量
    SR_Vec3f operator-() const;

    // 向量和实数相乘
    SR_Vec3f operator*(float t) const;
    friend SR_Vec3f operator*(float t, const SR_Vec3f &v);

    // 向量和实数相除
    SR_Vec3f operator/(float t) const;

    // 向量长度
    float length() const;

    // 向量点乘
    float dot(const SR_Vec3f &v) const;

    // 和另一个向量的夹角
    float angle(const SR_Vec3f &v) const;

    // 和另一个向量叉乘的值
    float crossValue(const SR_Vec3f &v) const;

    // 向量叉乘
    SR_Vec3f cross(const SR_Vec3f &v) const;

    // 将当前向量归一化
    void normalize();

    // 打印向量
    void printValue(const char *title = nullptr) const;

public:
    float x;
    float y;
    float z;
};

//-----------------------------------------------------------------------------

#endif // __SR_VEC3F_H__
