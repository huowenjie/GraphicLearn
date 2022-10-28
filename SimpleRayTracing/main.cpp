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
 * BlinnPhong Shading 模型
 * L = ka * Ia + kd * I * max(0, n · l) + ks * I *max(0, n · h)^n
 */
static float blinnPhongShading(
    const SRT_Vec3f &lightDir,
    const SRT_Vec3f &normal,
    const SRT_Vec3f &viewDir
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

    // 定义摄像机注视方向（固定）
    SRT_Vec3f lookDir(0.0f, 0.0f, -1.0f);

    // 定义光源位置 默认是白光
    SRT_Vec3f light(2.0f, 4.0f, 4.0f);
    SRT_Color lightColor(1.0f);

    // 计算光线方向
    SRT_Vec3f rayDir = lookDir + SRT_Vec3f(st.x, st.y, 0.0f);
    rayDir.normalize();

    // 定义两个球
    Sphere sps[] {
        Sphere(SRT_Vec3f(1.0f, 1.0f, 1.0f), SRT_Color(1.0f, 0.0f, 0.0f), 1.0f),
        Sphere(SRT_Vec3f(-1.2f, 0.0f, 0.0f), SRT_Color(0.3f, 0.3f, 1.0f), 1.0f)
    };

    bool isHit = false;
    int index = -1;
    float mint = 0.0f;
    float t = 0.0f;

    for (int i = 0; i < 2; i++) {
        int hit = sphereHit(camera, rayDir, sps[i].center, sps[i].r, &t, nullptr);
        if (hit >= 0) {
            if (i == 0) {
                mint = t;
            }

            if (t < mint) {
                mint = t;
            }

            isHit = true;
            index = i;
        }
    }

    // 相交点法线向量
    if (isHit) {
        SRT_Vec3f normal;

        // 计算交点
        SRT_Vec3f hitPoint = camera + t * rayDir;

        // 计算光源方向
        SRT_Vec3f lightDir = light - hitPoint;
        lightDir.normalize();

        bool isShadow = false;

        for (int i = 0; i < 2; i++) {
            if (i == index) {
                continue;
            }

            /* 这里的碰撞点有问题 TODO t 不一定与 hitPoint 一致 */

            /* 将光线原点定为当前对象的射线碰撞点，光线方向为光源方向，检测光线是否和其他对象碰撞，如果碰撞，则代表当前
               碰撞位置则为阴影像素 */
            if (sphereHit(hitPoint, lightDir, sps[i].center, sps[i].r, nullptr, nullptr) >= 0) {
                isShadow = true;
                break;
            }
        }

        if (!isShadow) {
            /* 计算法线向量 */
            normal = hitPoint - sps[index].center;
            normal.normalize();

            /* 计算视线 */
            SRT_Vec3f viewDir = camera - hitPoint;
            viewDir.normalize();

            /* 着色 */
            float lbs = blinnPhongShading(lightDir, normal, viewDir);
            color = lbs * lightColor * sps[index].color;

            /* 将颜色值限定在 0.0~1.0 */
            color.clamp();
        } else {
            color = 0.04f * color;
        }
    } else {
        color = SRT_Color(uv.y);
    }
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

float blinnPhongShading(
    const SRT_Vec3f &lightDir,
    const SRT_Vec3f &normal,
    const SRT_Vec3f &viewDir
)
{
    float ka = 0.2f;
    float kd = 1.0f;
    float ks = 0.8f;

    float ia = 0.2f;
    float i = 0.6f;

    SRT_Vec3f h = viewDir + lightDir;
    h.normalize();

    float diff = kd * i * srtMaxf(0.0f, lightDir.dot(normal));
    float ambient = ka * ia;
    float specular = ks * i * std::pow(srtMaxf(0.0f, normal.dot(h)), 32.0f);

    float blinnPhong = diff + ambient + specular;
    return blinnPhong;
}