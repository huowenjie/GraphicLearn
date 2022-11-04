#ifndef __SR_MAT4X4F_H__
#define __SR_MAT4X4F_H__

#include "sr_vec4f.h"

//-----------------------------------------------------------------------------
// 4 x 4 方阵
//-----------------------------------------------------------------------------

class SR_Mat4x4f
{
public:
    SR_Mat4x4f();
    SR_Mat4x4f(float elem);
    SR_Mat4x4f(float *elems);
    SR_Mat4x4f(float (*elems)[4]);
    SR_Mat4x4f(const SR_Mat4x4f &mat);
    ~SR_Mat4x4f();

public:
    // 单位矩阵
    static SR_Mat4x4f unitMatrix();

    // 零矩阵
    static SR_Mat4x4f zeroMatrix();

    // 转置矩阵
    static SR_Mat4x4f transpose(const SR_Mat4x4f &mat);

    // 逆矩阵
    static SR_Mat4x4f inverse(const SR_Mat4x4f &mat);

    // 4x4 行列式
    static float determinant(const SR_Mat4x4f &mat);

public:
    SR_Mat4x4f & operator=(const SR_Mat4x4f &v);

    // 矩阵和
    SR_Mat4x4f operator+(const SR_Mat4x4f &mat) const;
    SR_Mat4x4f operator-(const SR_Mat4x4f &mat) const;

    // 矩阵和标量相乘
    SR_Mat4x4f operator*(float val) const;
    friend SR_Mat4x4f operator*(float val, const SR_Mat4x4f &mat);

    // 矩阵相乘
    SR_Mat4x4f operator*(const SR_Mat4x4f &mat) const;

    // 矩阵和向量相乘
    SR_Vec4f operator*(const SR_Vec4f &vec) const;

    // 矩阵取值
    float *operator[](int i);

    // 逆矩阵
    SR_Mat4x4f operator-() const;

public:
    // 转置矩阵
    SR_Mat4x4f transpose() const;

    // 求 4 x 4 行列式
    float determinant() const;

    // 打印矩阵
    void printValue(const char *title = nullptr) const;

public:
    union {
        /**
         * [ m00, m01, m02, m03 ]
         * [ m10, m11, m12, m13 ]
         * [ m20, m21, m22, m23 ]
         * [ m30, m31, m32, m33 ]
         */
        struct {
            float m00;
            float m01;
            float m02;
            float m03;
            float m10;
            float m11;
            float m12;
            float m13;
            float m20;
            float m21;
            float m22;
            float m23;
            float m30;
            float m31;
            float m32;
            float m33;
        };

        float mat[16];
        float matij[4][4];
    };
};

//-----------------------------------------------------------------------------

#endif // __SR_MAT4X4F_H__