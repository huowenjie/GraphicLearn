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

public: // 常用的 3D 线性变换矩阵

    // 缩放变换
    static SR_Mat4x4f scaleMatrix(float sx, float sy, float sz);

    // 围绕 z 轴的旋转的旋转矩阵
    static SR_Mat4x4f rotateZMatrix(float theta);

    // 围绕 X 轴的旋转的旋转矩阵
    static SR_Mat4x4f rotateXMatrix(float theta);

    // 围绕 Y 轴的旋转的旋转矩阵
    static SR_Mat4x4f rotateYMatrix(float theta);

    // 平移矩阵
    static SR_Mat4x4f translateMatrix(float dx, float dy, float dz);

    /**
     * 体积盒投影矩阵
     * 将 [xl, xh] x [yl, yh] x [zl, zh] 投影到 [xpl, xph] x [ypl, yph] x [zpl, zph]
     * 
     * l 和 h 构成了待变换窗口的包围盒
     * lp 和 hp 构成了目标窗口的包围盒
     * 
     * l 的值为 xmin ymin zmin
     * h 的值为 xmax ymax zmax
     * 
     * lp 和 hp 同理
     */
    static SR_Mat4x4f volumeMapMatrix(
        const SR_Vec3f &l,
        const SR_Vec3f &h,
        const SR_Vec3f &lp,
        const SR_Vec3f &hp
    );

    /**
     * 将标准化坐标投影到屏幕空间-视口变换
     * 
     * width 屏幕宽度（像素）
     * height 屏幕高度（像素）
     */
    static SR_Mat4x4f viewportMatrix(int width, int height);

    /**
     * 摄像机视觉变换矩阵
     * 
     * pos 摄像机位置
     * target 目标位置
     */
    static SR_Mat4x4f cameraMatrix(const SR_Vec3f &pos, const SR_Vec3f &target);

    /**
     * 透视投影矩阵
     * 
     * 将体积盒 [xl, xh] x [yl, yh] x [zl, zh] 中的物体
     * 进行透视变换，同时投影到规范化坐标系统 [-1, 1] 上
     */
    static SR_Mat4x4f perspectiveMatrix(const SR_Vec3f &l, const SR_Vec3f &h);

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