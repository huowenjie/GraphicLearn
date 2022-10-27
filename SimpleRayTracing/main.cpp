#include <cmath>
#include <cstdio>
#include "srt_window.h"
#include "srt_vec3f.h"
#include "srt_global.h"

class Sphere
{
public:
    Sphere(const SRT_Vec3f &center, const SRT_Color &color, float r)
    {
        this->center = center;
        this->color = color;
        this->r = r;
    }

public:
    SRT_Vec3f center;
    SRT_Color color;
    float r;
};

// 逐像素访问，效果类似于片段着色器
static void visitPixels(const SRT_Vec2f &resolution, const SRT_Vec2f &uv, SRT_Color &color);

/**
 * 计算光线和球的交点
 * 返回值： -1 不相交，0 有一个交点，1 有两个交点
 * 
 * origin 光线原点
 * rayDir 光线方向
 * center 球心
 * r 半径
 * t1 射线和球相交时的参数解1
 * t2 射线和球相交时的参数解2
 */
static int sphereHit(
    const SRT_Vec3f &origin,
    const SRT_Vec3f &rayDir,
    const SRT_Vec3f &center,
    float r,
    float *t1,
    float *t2
);

/**
 * Lambertian Shading 模型
 * L = kd I max(0, n · l)
 * 
 * kd 是漫反射因子
 * I 是光照强度
 * n 是光线与物体交点法线
 * l 是光源方向
 */
static float lambertianShading(
    const SRT_Vec3f &lightDir,
    const SRT_Vec3f &normal
);

int main(int argc, char *argv[])
{
    SRT_Window window;

    window.initialize();
    window.setForeachPixelCallback(visitPixels);
    window.render();
    window.terminate();
    return 0;
}

void visitPixels(const SRT_Vec2f &resolution, const SRT_Vec2f &uv, SRT_Color &color)
{
    // 将 x 轴的单位进行按比例缩放并将坐标轴向右上移动0.5个单位
    float radio = resolution.x / resolution.y;
    SRT_Vec2f st((uv.x - 0.5f) * radio, uv.y - 0.5f);

    // 定义摄像机位置也是光线的原点
    SRT_Vec3f camera(0.0f, 0.0f, 5.0f);
    SRT_Vec3f imagePlane(st.x, st.y, 0.0f);

    // 定义光源位置 默认是白光
    SRT_Vec3f light(3.0f, 4.0f, 0.0f);

    // 计算光线方向
    SRT_Vec3f rayDir = imagePlane - camera;
    rayDir.normalize();

    // 定义两个球
    Sphere sps[] {
        Sphere(SRT_Vec3f(0.0f), SRT_Color(0.7f, 0.0f, 0.0f), 0.3f),
        Sphere(SRT_Vec3f(0.2f, 0.2f, 1.0f), SRT_Color(0.0f, 0.3f, 1.0f), 0.3f)
    };

    bool isHit = false;
    int index = 0;
    float mint = -10000.0f;

    for (int i = 0; i < 2; i++) {
        float t = 0.0f;
        int hit = sphereHit(camera, rayDir, sps[i].center, sps[i].r, &t, nullptr);

        if (hit >= 0) {
            if (t < mint) {
                mint = t;
                index = i;
                isHit = true;
            }
        }
    }

    if (isHit) {
        color = sps[index].color;
    }

    // 相交点法线向量
    // SRT_Vec3f normal;

    // float t1 = 0.0f;
    // float t2 = 0.0f;
    // float t = 0.0f;

    // int hit1 = sphereHit(camera, rayDir, sps[0].center, sps[0].r, &t1, nullptr);
    // if (hit1 >= 0) {
    //     color = sps[0].color;
    //     t = t1;

    //     //normal = center1;
    // }

    // int hit2 = sphereHit(camera, rayDir, sps[1].center, sps[1].r, &t2, nullptr);
    // if (hit2 >= 0) {
    //     if (t2 < t1) {
    //         color = sps[1].color;
    //         t = t2;
    //         //normal = center2;
    //     }
    // }

    // if (hit1 >= 0 || hit2 >=0) {
    //     // 计算交点
    //     SRT_Vec3f hitPoint = camera + t * rayDir;

    //     // 计算光源方向
    //     SRT_Vec3f lightDir = light - hitPoint;
    //     lightDir.normalize();

    //     // 计算法线向量
    //     normal = hitPoint - normal;
    //     normal.normalize();

    //     // 着色
    //     float lbs = lambertianShading(lightDir, normal);
    //     color = lbs * color;
    // }
}

/**
 * 设有任意点 p (e + td)，e 是摄像机原点，d 是视线方向，t 为可变参数[0, 无穷大)
 * 
 * 根据球的表达式得：
 * (p - c) · (p - c) - R^2 = 0
 * 
 * 设 st = e + td，并将其带入表达式得：
 * (d · d)t2 + 2d · (e − c)t + (e − c) · (e − c) − R2 = 0
 * 
 * t = −d · (e − c) ± sqrt((d · (e − c))2 − (d · d) ((e − c) · (e − c) − R2)) / (d · d)
 */
int sphereHit(
    const SRT_Vec3f &origin,
    const SRT_Vec3f &rayDir,
    const SRT_Vec3f &center,
    float r,
    float *t1,
    float *t2
)
{
    // 首先计算 b2 - 4ac
    float disc = 0.0f;
    float dd = rayDir.dot(rayDir);
    SRT_Vec3f ec = origin - center;

    disc = rayDir.dot(ec);
    disc = disc * disc;

    float tmp = dd;
    tmp = tmp * (ec.dot(ec) - r * r);
    disc -= tmp;

    if (disc < -1e-6f) {
        return -1;
    }

    if (disc > 1e-6f) {
        if (t1) {
            *t1 = (-rayDir.dot(ec) - std::sqrt(disc)) / dd;
        }

        if (t2) {
            *t2 = (-rayDir.dot(ec) + std::sqrt(disc)) / dd;
        }
        return 1;
    }

    tmp = -rayDir.dot(ec) / dd;
    if (t1) {
        *t1 = tmp;
    }
    if (t2) {
        *t2 = tmp;
    }
    return 0;
}

float lambertianShading(
    const SRT_Vec3f &lightDir,
    const SRT_Vec3f &normal
)
{
    float kd = 0.3f;
    float i = 10.0f;
    float lsd = kd * i * srtMaxf(0.0f, lightDir.dot(normal));
    return srtClampf(lsd, 1.0f, 0.0f);
}
