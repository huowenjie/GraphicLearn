#include <cmath>
#include "srt_window.h"
#include "srt_vec3f.h"
#include "srt_global.h"

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
    float &t1,
    float &t2
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
    /**
     * 光线追踪的三步
     * 
     * 1.计算视图射线
     * 2.找到第一个与射线相交的对象并计算其表面法线
     * 3.利用 1、2 获取的信息对像素点着色
     */

    // 将 x 轴的单位进行按比例缩放并将坐标轴向右上移动0.5个单位
    float radio = resolution.x / resolution.y;
    SRT_Vec2f st((uv.x - 0.5f) * radio, uv.y - 0.5f);

    // 定义摄像机位置也是光线的原点
    SRT_Vec3f camera(0.0f, 0.0f, 5.0f);
    SRT_Vec3f imagePlane(st.x, st.y, 0.0f);

    // 计算光线方向
    SRT_Vec3f rayDir = imagePlane - camera;
    rayDir.normalize();

    // 直接计算 t
    float t1 = 0.0f;
    float t2 = 0.0f;
    int hit = sphereHit(camera, rayDir, SRT_Vec3f(0.0f), 0.3f, t1, t2);

    // 如果 t >= 0 光线被碰撞，则显示当前像素点
    if (hit >= 0) {
        color.r = 0.4f;
        color.b = 0.4f;
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
    float &t1,
    float &t2
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
        t1 = (-rayDir.dot(ec) + std::sqrt(disc)) / dd;
        t1 = (-rayDir.dot(ec) - std::sqrt(disc)) / dd;
        return 1;
    }

    t1 = -rayDir.dot(ec) / dd;
    t2 = t1;
    return 0;
}
