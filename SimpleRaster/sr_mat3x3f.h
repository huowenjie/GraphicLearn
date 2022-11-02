#ifndef __SR_MAT3F_H__
#define __SR_MAT3F_H__

#include "sr_vec3f.h"

//-----------------------------------------------------------------------------
// 3 x 3 方阵，行主序存储
//-----------------------------------------------------------------------------

class SR_Mat3x3f
{
public:
    SR_Mat3x3f();
    SR_Mat3x3f(float elem);
    SR_Mat3x3f(float *elems);
    SR_Mat3x3f(float (*elems)[3]);
    SR_Mat3x3f(const SR_Mat3x3f &mat);
    ~SR_Mat3x3f();

public:
    // 单位矩阵
    static SR_Mat3x3f unitMatrix();

    // 零矩阵
    static SR_Mat3x3f zeroMatrix();

    // 转置矩阵
    static SR_Mat3x3f transpose(const SR_Mat3x3f &mat);

    // 逆矩阵
    static SR_Mat3x3f inverse(const SR_Mat3x3f &mat);

public:
    SR_Mat3x3f & operator=(const SR_Mat3x3f &v);

    // 矩阵和
    SR_Mat3x3f operator+(const SR_Mat3x3f &mat) const;
    SR_Mat3x3f operator-(const SR_Mat3x3f &mat) const;

    // 矩阵和标量相乘
    SR_Mat3x3f operator*(float val) const;
    friend SR_Mat3x3f operator*(float val, const SR_Mat3x3f &mat);

    // 矩阵相乘
    SR_Mat3x3f operator*(const SR_Mat3x3f &mat) const;

    // 矩阵和向量相乘
    SR_Vec3f operator*(const SR_Vec3f &vec) const;

    // 矩阵取值
    float *operator[](int i);

    // 逆矩阵
    SR_Mat3x3f operator-() const;

public:
    // 转置矩阵
    SR_Mat3x3f transpose() const;

    // 打印矩阵
    void printValue(const char *title = nullptr) const;

public:
    union {
        /**
         * [ m00, m01, m02 ]
         * [ m10, m11, m12 ]
         * [ m20, m21, m22 ]
         */
        struct {
            float m00;
            float m01;
            float m02;
            float m10;
            float m11;
            float m12;
            float m20;
            float m21;
            float m22;
        };

        float mat[9];
        float matij[3][3];
    };
};

//-----------------------------------------------------------------------------

#endif // __SR_MAT3F_H__