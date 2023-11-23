#ifndef __SR_FRAGMENT_H__
#define __SR_FRAGMENT_H__

#include "sr_indexmesh.h"

//-----------------------------------------------------------------------------
// 片段
//-----------------------------------------------------------------------------

class SR_Fragment
{
public:
    SR_Fragment();
    ~SR_Fragment();

public:
    // 重心坐标插值 z 为透视矫正系数
    void interpolateFragCoord(
        float alpha,
        float beta,
        float gama,
        float z,
        const SR_Vec3f &a,
        const SR_Vec3f &b,
        const SR_Vec3f &c
    );

    void interpolateFragColor(
        float alpha,
        float beta,
        float gama,
        float z,
        const SR_Color &a,
        const SR_Color &b,
        const SR_Color &c
    );

    void interpolateNormal(
        float alpha,
        float beta,
        float gama,
        float z,
        const SR_Vec3f &a,
        const SR_Vec3f &b,
        const SR_Vec3f &c
    );

    void interpolateUV(
        float alpha,
        float beta,
        float gama,
        float z,
        const SR_Vec2f &a,
        const SR_Vec2f &b,
        const SR_Vec2f &c
    );

    void interpolateFragGlobal(
        float alpha,
        float beta,
        float gama,
        float z,
        const SR_Vec3f &a,
        const SR_Vec3f &b,
        const SR_Vec3f &c
    );

    // 设置片元坐标
    void setFragCoord(const SR_Vec3f &coord);

    // 设置片元颜色
    void setFragColor(const SR_Color &color);

    // 设置片元法线
    void setFragNormal(const SR_Vec3f &normal);

    // 设置片元所属面法线
    void setFragSurfaceNormal(const SR_Vec3f &normal);

    // 设置片元nv坐标
    void setFragUV(const SR_Vec2f &uv);

    // 设置当前窗口分辨率
    void setResolution(float width, float height);

    // 设置片元对应的世界坐标
    void setFragGlobal(const SR_Vec3f &global);

    // 获取片元坐标
    SR_Vec3f getFragCoord() const;

    // 获取片元颜色
    SR_Color getFragColor() const;

    // 获取片元法线
    SR_Vec3f getFragNormal() const;

    // 获取片元面法线
    SR_Vec3f getFragSurfaceNormal() const;

    // 获取 UV 坐标
    SR_Vec3f getFragUV() const;

    // 获取窗口分辨率
    SR_Vec3f getResolution() const;

    // 获取片元对应的世界坐标
    SR_Vec3f getFragGlobal() const;

protected:
    // 片元坐标
    SR_Vec3f fragCoord;

    // 片元颜色
    SR_Color fragColor;

    // 片元法线
    SR_Vec3f normal;

    // 片元所属面法线
    SR_Vec3f surfaceNormal;

    // uv 坐标
    SR_Vec2f uv;

    // 当前窗口分辨率
    SR_Vec3f resolution;

    // 片元对应的世界坐标，用于和世界坐标系的参数进行运算
    SR_Vec3f fragGlobal;
};

//-----------------------------------------------------------------------------

#endif // __SR_FRAGMENT_H__
