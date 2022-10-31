#include <cmath>
#include <cstdio>

#include "srt_window.h"
#include "srt_vec3f.h"
#include "srt_global.h"

// 球
class Sphere
{
public:
    Sphere(const SRT_Vec3f &center, const SRT_Color &color, float r)
    {
        this->center = center;
        this->color = color;
        this->r = r;
        this->t1 = 0.0f;
        this->t2 = 0.0f;
    }

public:
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
     * 
     * 计算光线和球的交点
     * 返回值： -1 不相交，0 有一个交点，1 有两个交点
     * 
     * 参数
     * origin 光线原点
     * rayDir 光线方向
     *
     * 调用本函数修改的成员：
     * t1 射线和球相交时的参数解1
     * t2 射线和球相交时的参数解2
     * 且 t1 <= t2
     */
    int sphereHit(const SRT_Vec3f &origin, const SRT_Vec3f &rayDir)
    {
        // 计算 disc = b2 - 4ac
        float disc = 0.0f;
        float dd = rayDir.dot(rayDir);
        SRT_Vec3f ec = origin - center;

        disc = rayDir.dot(ec);
        disc = disc * disc;

        float tmp = dd;
        tmp = tmp * (ec.dot(ec) - r * r);
        disc -= tmp;

        if (disc < -1e-6f) {
            t1 = 0.0f;
            t2 = 0.0f;
            return -1;
        }

        if (disc > 1e-6f) {
            t1 = (-(rayDir.dot(ec)) - std::sqrt(disc)) / dd;
            t2 = (-(rayDir.dot(ec)) + std::sqrt(disc)) / dd;
            return 1;
        }

        tmp = -(rayDir.dot(ec)) / dd;
        t1 = tmp;
        t2 = tmp;
        return 0;
    }

public:
    SRT_Vec3f center;
    SRT_Color color;
    float r;
    float t1;
    float t2;
};

// 用于辅助运算的 struct
struct Matrix {
    float a;
    float b;
    float c;
    float d;
    float e;
    float f;
    float g;
    float h;
    float i;
    float j;
    float k;
    float l;
};

// 三角形
class Triangle
{
public:
    Triangle(const SRT_Vec3f *points, const SRT_Color &color)
    {
        for (int i = 0; i < 3; i++) {
            this->points[i] = points[i];
        }
        this->color = color;
    }

public:
    /**
     * 设有任意点 p (e + td)，e 是摄像机原点，d 是视线方向，t 为可变参数[0, 无穷大)
     * 有参数方程组 e + td = a + β(b − a) + γ(c − a),
     * 直接利用克拉默法则解出 t、β、γ
     * 
     * 参数
     * origin 光线原点
     * rayDir 光线方向
     *
     * 调用本函数修改的成员：
     * t
     * 
     * 返回 -1，则不相交；返回 1 则相交
     */
    int rectangleHit(const SRT_Vec3f &origin, const SRT_Vec3f &rayDir)
    {
        SRT_Vec3f ab = points[0] - points[1];
        SRT_Vec3f ac = points[0] - points[2];
        SRT_Vec3f ae = points[0] - origin;

        Matrix mat = {
            ab.x, ab.y, ab.z,
            ac.x, ac.y, ac.z,
            rayDir.x, rayDir.y, rayDir.z,
            ae.x, ae.y, ae.z
        };

        float m = mat.a * (mat.e * mat.i - mat.h * mat.f) +
            mat.b * (mat.g * mat.f - mat.d * mat.i) + mat.c * (mat.d * mat.h - mat.e * mat.g);
        
        if (std::abs(m) < 1e-6) {
            return -1;
        }

        float gama = mat.i * (mat.a * mat.k - mat.j * mat.b) + 
            mat.h * (mat.j * mat.c - mat.a * mat.l) + mat.g * (mat.b * mat.l - mat.k * mat.c);
        gama /= m;

        if (gama < 0.0f || gama > 1.0f) {
            return -1;
        }

        float beta = mat.j * (mat.e * mat.i - mat.h * mat.f) + 
            mat.k * (mat.g * mat.f - mat.d * mat.i) + mat.l * (mat.d * mat.h - mat.e * mat.g);
        beta /= m;

        if (beta < 0.0f || beta > 1.0f - gama) {
            return -1;
        }

        float tmp = mat.f * (mat.a * mat.k - mat.j * mat.b) + 
            mat.e * (mat.j * mat.c - mat.a * mat.l) + mat.d * (mat.b * mat.l - mat.k * mat.c);
        tmp /= m;
        tmp = -tmp;

        if (tmp < 0.0f) {
            return -1;
        }

        this->t = tmp;
        return 1;
    }

public:
    SRT_Vec3f points[3];
    SRT_Color color;
    float t;
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

#define AMBIENT (0.4f * 0.2f)
#define DIFFUSE(i) (1.0f * (i))
#define SPECULAR(i) (0.8f * (i))

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
    SRT_Vec3f light(4.0f, 6.0f, 4.0f);
    SRT_Color lightColor(1.0f);

    // 计算光线方向
    SRT_Vec3f rayDir = lookDir + SRT_Vec3f(st.x, st.y, 0.0f);
    rayDir.normalize();

    // 定义两个球
    Sphere sps[] {
        Sphere(SRT_Vec3f(1.0f, 0.8f, 1.0f), SRT_Color(1.0f, 0.0f, 0.0f), 1.0f),
        Sphere(SRT_Vec3f(-1.5f, 0.0f, 0.0f), SRT_Color(0.3f, 0.3f, 1.0f), 1.0f)
    };

    // 定义三角形三个顶点
    SRT_Vec3f points[3] = {
        SRT_Vec3f(0.0f, 0.0f, 0.0f),
        SRT_Vec3f(1.0f, 0.0f, 0.0f),
        SRT_Vec3f(1.0f, 1.0f, 0.0f)
    };
    Triangle bottom(points, SRT_Color(1.0f, 1.0f, 0.0f));

    bool isHit = false;
    int index = -1;
    float mint = 0.0f;

    for (int i = 0; i < 2; i++) {
        int hit = sps[i].sphereHit(camera, rayDir);
        if (hit >= 0) {
            if (i == 0) {
                mint = sps[i].t1;
                index = i;
                isHit = true;
            }

            if (!isHit || sps[i].t1 < mint) {
                mint = sps[i].t1;
                index = i;
                isHit = true;
            }
        }
    }

    bool rendTriangle = false;

    if (bottom.rectangleHit(camera, rayDir) > 0) {
        if (bottom.t < mint) {
            mint = bottom.t;
            rendTriangle = true;
        }
    }

    if (rendTriangle) {
        color = bottom.color;
    } else {
        color = SRT_Color(uv.y);
    }

    // 相交点法线向量
    // if (isHit) {
    //     SRT_Vec3f normal;

    //     // 计算交点
    //     SRT_Vec3f hitPoint = camera + mint * rayDir;

    //     // 计算光源方向
    //     SRT_Vec3f lightDir = light - hitPoint;
    //     lightDir.normalize();

    //     bool isShadow = false;

    //     for (int i = 0; i < 2; i++) {
    //         /* 将光线原点定为当前对象的射线碰撞点，光线方向为光源方向，检测光线是否和其他对象碰撞，如果碰撞，则代表当前
    //            碰撞位置为阴影像素 */
    //         if (i != index && sps[i].sphereHit(hitPoint, lightDir) >= 0) {
    //             /* 这里判断 st > 0.0 是为了表示和阴影光线相交的图形必须位于碰撞点沿着光线的正方向移动的方向 */
    //             if (sps[i].t1 > 0.0f) {
    //                 isShadow = true;
    //                 break;
    //             }
    //         }
    //     }

    //     if (!isShadow) {
    //         /* 计算法线向量 */
    //         normal = hitPoint - sps[index].center;
    //         normal.normalize();

    //         /* 计算视线 */
    //         SRT_Vec3f viewDir = camera - hitPoint;
    //         viewDir.normalize();

    //         /* 着色 */
    //         float lbs = blinnPhongShading(lightDir, normal, viewDir);
    //         color = lbs * lightColor * sps[index].color;

    //         /* 将颜色值限定在 0.0~1.0 */
    //         color.clamp();
    //     } else {
    //         color = AMBIENT * lightColor * sps[index].color;
    //     }
    // } else {
    //     color = SRT_Color(uv.y);
    // }
}

float blinnPhongShading(
    const SRT_Vec3f &lightDir,
    const SRT_Vec3f &normal,
    const SRT_Vec3f &viewDir
)
{
    float i = 0.6f;

    SRT_Vec3f h = viewDir + lightDir;
    h.normalize();

    float diff = DIFFUSE(i) * srtMaxf(0.0f, lightDir.dot(normal));
    float specular = SPECULAR(i) * std::pow(srtMaxf(0.0f, normal.dot(h)), 32.0f);
    float blinnPhong = diff + AMBIENT + specular;
    return blinnPhong;
}