#ifndef __SR_MAT2X2F_H__
#define __SR_MAT2X2F_H__

#include "sr_vec2f.h"

//-----------------------------------------------------------------------------
// 2 x 2 方阵，行主序存储
//-----------------------------------------------------------------------------

class SR_Mat2x2f
{
public:
    SR_Mat2x2f();
    SR_Mat2x2f(float elem);
    SR_Mat2x2f(float *elems);
    SR_Mat2x2f(float (*elems)[2]);
    SR_Mat2x2f(const SR_Mat2x2f &mat);
    ~SR_Mat2x2f();

public:
    // 单位矩阵
    static SR_Mat2x2f unitMatrix();

    // 零矩阵
    static SR_Mat2x2f zeroMatrix();

    // 转置矩阵
    static SR_Mat2x2f transpose(const SR_Mat2x2f &mat);

    // 逆矩阵
    static SR_Mat2x2f inverse(const SR_Mat2x2f &mat);

public:
    SR_Mat2x2f & operator=(const SR_Mat2x2f &mat);

    // 矩阵和
    SR_Mat2x2f operator+(const SR_Mat2x2f &mat) const;
    SR_Mat2x2f operator-(const SR_Mat2x2f &mat) const;

    // 矩阵和标量相乘
    SR_Mat2x2f operator*(float val) const;
    friend SR_Mat2x2f operator*(float val, const SR_Mat2x2f &mat);

    // 矩阵相乘
    SR_Mat2x2f operator*(const SR_Mat2x2f &mat) const;

    // 矩阵和向量相乘
    SR_Vec2f operator*(const SR_Vec2f &vec) const;

    // 矩阵取值
    float *operator[](int i);

    // 逆矩阵
    SR_Mat2x2f operator-() const;

public:
    // 转置矩阵
    SR_Mat2x2f transpose() const;

    // 打印矩阵
    void printValue(const char *title = nullptr) const;

public:
    union {
        struct {
            float m00;
            float m01;
            float m10;
            float m11;
        };

        float mat[4];
        float matij[2][2];
    };
};

//-----------------------------------------------------------------------------

#endif // __SR_MAT2X2F_H__
