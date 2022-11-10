#include <cmath>
#include <cstdio>

#include "sr_window.h"
#include "sr_mat4x4f.h"
#include "sr_global.h"
#include "sr_indexmesh.h"

static void update(SR_Window &window);

/**
 * 齐次裁剪空间的定义：
 * -x + lw = 0,
 *  x - rw = 0,
 * -y + bw = 0,
 *  y - tw = 0,
 * -z + nw = 0,
 *  z - fw = 0
 * 
 * 判断顶点是否在平面内，plane 为平面法线的向量表示
 * 比如 near 平面的法线为 SR_Vec4f(0.0f, 0.0f, -1.0f, 1.0f);
 * 
 * 返回值为在齐次空间下点和平面的距离，如果距离 < 0.0，点在平面内侧；
 * 否则在平面外侧
 */
static float insideInPlane(const SR_Vec4f &n, const SR_Vec4f &p);

int main(int argc, char *argv[])
{
    SR_Window window;

    window.initialize();

    window.setUpdate(update);
    window.render();

    window.terminate();

    return 0;
}

/**
 * 设一个三角形 abc 顶点逆时针定义
 * 
 * 情况一、
 * abc 完全在裁剪平面外，则不需要渲染
 * 
 * 情况二、
 * abc 完全在裁剪平面内，则直接渲染
 * 
 * 情况三、
 * abc 某一个顶点在裁剪平面外，另外两个顶点位于裁剪平面内，则需要将三角形裁成一个四边形
 * ，比如 a 点恰好位于平面外，ab 和 ac 与该裁剪平面分别相交于 m 和 n，那么四边形 mbcn
 * 则由 mbc 和 cnm 两个三角形组成
 * 
 * 情况四、
 * abc 两个顶点在裁剪平面外，另外一个顶点位于裁剪平面内，则裁剪的结果依旧是一个三角形
 * ，比如 bc 在外，a 在内，ab 和 ac 分别相交于 m 和 n，那么裁剪的结果即为三角形 amn
 * 
 * 在齐次坐标系中进行裁剪
 * 
 * 标准齐次裁剪空间的定义：
 * -x + lw = 0,
 *  x - rw = 0,
 * -y + bw = 0,
 *  y - tw = 0,
 * -z + nw = 0,
 *  z - fw = 0
 * 
 * 判断三角形和平面的交点首先要判断线段和平面的交点
 * 
 * 假设 q 为平面上一点，n 为该平面的法线向量，存在点 p, 使得 n · (p - q) = 0，
 * 那么 p 点就位于该平面上，即有
 * 
 * f(p) = n · (p - q)
 * 
 * 进一步推导有
 * 
 * f(p) = n · p - n · q，由于 n 和 q 是常量，所以令 D = -n · q，则有
 * f(p) = n · p + D
 * 
 * 如果 f(p) < 0 那么 p 点在平面内侧
 * 如果 f(p) > 0 那么 p 点在平面外侧
 * 
 * 假设有 a 和 b 两点，线段 ab 与平面的交点 p 满足 p = a + t(b − a)
 * 带入 f(p) = 0 得 t = (n · a + D) / (n · (a - b))
 */
void update(SR_Window &window)
{
    int w = window.getWidth();
    int h = window.getHeight();

    static float distance = 2.0f;
    static float target = 0.0f;

    SR_Mat4x4f vp = SR_Mat4x4f::viewportMatrix(w, h);
    SR_Mat4x4f per = SR_Mat4x4f::perspectiveMatrix(
        SR_Vec3f(-1.0f, -1.0f, -1.0f),
        SR_Vec3f(1.0f, 1.0f, 1.0f)
    );
    SR_Mat4x4f cam = SR_Mat4x4f::cameraMatrix(
        SR_Vec3f(0.0f, 0.0f, distance),
        SR_Vec3f(0.0f, 0.0f, target)
    );
    SR_Mat4x4f move = SR_Mat4x4f::translateMatrix(0.0f, 0.0f, 0.7f);

    // if (distance > 1.0f) {
    //     distance -= 0.02f;
    //     target -= 0.02f;
    // }

    // 透视变换至齐次裁剪空间
    SR_Mat4x4f proj = per * cam;
    SR_IndexMesh mesh;

    mesh.addVertex(SR_Vec3f(-0.5f, -0.5f, 0.5f));
    mesh.addVertex(SR_Vec3f(0.5f, -0.5f, 0.5f));
    mesh.addVertex(SR_Vec3f(0.5f, -0.5f, -0.5f));
    mesh.addVertex(SR_Vec3f(-0.5f, -0.5f, -0.5f));
    mesh.addIndexList(0, 1, 2);
    mesh.addIndexList(2, 3, 0);

    float clipFactor = distance - 1.0f;

    // 近平面参考线
    SR_Vec4f p1(-0.5f, -0.5f, clipFactor, 1.0f);
    SR_Vec4f p2(0.5f, -0.5f, clipFactor, 1.0f);
    SR_Vec4f p3(0.5f, 0.5f, clipFactor, 1.0f);
    SR_Vec4f p4(-0.5f, 0.5f, clipFactor, 1.0f);

    p1 = vp * proj * p1;
    p2 = vp * proj * p2;
    p3 = vp * proj * p3;
    p4 = vp * proj * p4;
    p1.homogenDivide();
    p2.homogenDivide();
    p3.homogenDivide();
    p4.homogenDivide();

    window.drawLine(
        SR_Vec2f(p1.x, p1.y),
        SR_Vec2f(p2.x, p2.y),
        SR_Color(1.0f)
    );

    window.drawLine(
        SR_Vec2f(p2.x, p2.y),
        SR_Vec2f(p3.x, p3.y),
        SR_Color(1.0f)
    );

    window.drawLine(
        SR_Vec2f(p3.x, p3.y),
        SR_Vec2f(p4.x, p4.y),
        SR_Color(1.0f)
    );

    window.drawLine(
        SR_Vec2f(p4.x, p4.y),
        SR_Vec2f(p1.x, p1.y),
        SR_Color(1.0f)
    );

    for (int i = 0; i < 2; i++) {
        const SR_TriangleIndexList &list = mesh.getIndexList(i);

        SR_Vec4f a = SR_Vec4f(mesh.getVertex(list.indexList[0]), 1.0f);
        SR_Vec4f b = SR_Vec4f(mesh.getVertex(list.indexList[1]), 1.0f);
        SR_Vec4f c = SR_Vec4f(mesh.getVertex(list.indexList[2]), 1.0f);

        a = proj * move * a;
        b = proj * move * b;
        c = proj * move * c;

        /**
         * 设一个三角形 abc 顶点逆时针定义
         * 
         * 情况一、
         * abc 完全在裁剪平面外，则不需要渲染
         * 
         * 情况二、
         * abc 完全在裁剪平面内，则直接渲染
         * 
         * 情况三、
         * abc 某一个顶点在裁剪平面外，另外两个顶点位于裁剪平面内，则需要将三角形裁成一个四边形
         * ，比如 a 点恰好位于平面外，ab 和 ac 与该裁剪平面分别相交于 m 和 n，那么四边形 mbcn
         * 则由 mbc 和 cnm 两个三角形组成
         * 
         * 情况四、
         * abc 两个顶点在裁剪平面外，另外一个顶点位于裁剪平面内，则裁剪的结果依旧是一个三角形
         * ，比如 bc 在外，a 在内，ab 和 ac 分别相交于 m 和 n，那么裁剪的结果即为三角形 amn
         */

        float da = insideInPlane(SR_Vec4f(0.0f, 0.0f, -1.0f, 1.0f), a);
        float db = insideInPlane(SR_Vec4f(0.0f, 0.0f, -1.0f, 1.0f), b);
        float dc = insideInPlane(SR_Vec4f(0.0f, 0.0f, -1.0f, 1.0f), c);

        // 分别比较 ab bc ac 三条边
        if (da > 0.0f && db > 0.0f && dc > 0.0f) {
            continue;
        } else if (
            (da <= 0.0f && db  > 0.0f && dc  > 0.0f) ||
            (da  > 0.0f && db <= 0.0f && dc  > 0.0f) ||
            (da  > 0.0f && db  > 0.0f && dc <= 0.0f)
        ) {
            // 首先找到在平面内部的点
            SR_Vec4f tmpInt;
            SR_Vec4f tmpExtA;
            SR_Vec4f tmpExtB;
            float d0 = 0.0f;
            float d1 = 0.0f;
            float d2 = 0.0f;

            if (da <= 0.0) {
                tmpInt = a;
                tmpExtA = b;
                tmpExtB = c;

                d0 = da;
                d1 = db;
                d2 = dc;
            } else if (db <= 0.0f) {
                tmpInt = b;
                tmpExtA = c;
                tmpExtB = a;

                d0 = db;
                d1 = dc;
                d2 = da;
            } else {
                tmpInt = c;
                tmpExtA = a;
                tmpExtB = b;

                d0 = dc;
                d1 = da;
                d2 = db;
            }

            float t0 = d0 / (d0 - d1);
            float t1 = d0 / (d0 - d2);

            // 求交点
            SR_Vec4f insA = SR_Vec4f::lerp(tmpInt, tmpExtA, t0);
            SR_Vec4f insB = SR_Vec4f::lerp(tmpInt, tmpExtB, t1);

            // 将顶点变换到 NDC
            a = vp * tmpInt;
            b = vp * insA;
            c = vp * insB;

            // 透视除法
            a.homogenDivide();
            b.homogenDivide();
            c.homogenDivide();

            // 颜色也要重新插值
            window.fillTriangle(
                SR_Vec2f(a.x, a.y),
                SR_Vec2f(b.x, b.y),
                SR_Vec2f(c.x, c.y),
                SR_Color(1.0f, 1.0f, 0.0f),
                t0 * SR_Color(0.0f, 1.0f, 1.0f),
                t1 * SR_Color(1.0f, 0.0f, 1.0f)
            );

            continue;
        } else if (
            (da  > 0.0f && db <= 0.0f && dc <= 0.0f) ||
            (da <= 0.0f && db  > 0.0f && dc <= 0.0f) ||
            (da <= 0.0f && db <= 0.0f && dc  > 0.0f)
        ) {
            // a、b、c 其中一点在平面外，其余两点在平面内

            // 首先找到在平面外部的点
            SR_Vec4f tmpOut;
            SR_Vec4f tmpIntA;
            SR_Vec4f tmpIntB;
            float d0 = 0.0f;
            float d1 = 0.0f;
            float d2 = 0.0f;

            if (da > 0.0) {
                tmpOut = a;
                tmpIntA = b;
                tmpIntB = c;

                d0 = da;
                d1 = db;
                d2 = dc;
            } else if (db > 0.0f) {
                tmpOut = b;
                tmpIntA = c;
                tmpIntB = a;

                d0 = db;
                d1 = dc;
                d2 = da;
            } else {
                tmpOut = c;
                tmpIntA = a;
                tmpIntB = b;

                d0 = dc;
                d1 = da;
                d2 = db;
            }

            float t0 = d0 / (d0 - d1);
            float t1 = d0 / (d0 - d2);

            // 求三角形和平面的交点
            SR_Vec4f insA = SR_Vec4f::lerp(tmpOut, tmpIntA, t0);
            SR_Vec4f insB = SR_Vec4f::lerp(tmpOut, tmpIntB, t1);

            // 原有的三角形 abc 被拆分为 insA-b-c 和 c-insB-insA
            tmpIntA = vp * tmpIntA;
            tmpIntB = vp * tmpIntB;
            insA = vp * insA;
            insB = vp * insB;

            tmpIntA.homogenDivide();
            tmpIntB.homogenDivide();
            insA.homogenDivide();
            insB.homogenDivide();

            // 直接渲染两个三角形
            window.fillTriangle(
                SR_Vec2f(insA.x, insA.y),
                SR_Vec2f(tmpIntA.x, tmpIntA.y),
                SR_Vec2f(tmpIntB.x, tmpIntB.y),
                SR_Color::lerp(SR_Color(1.0f, 1.0f, 0.0f), SR_Color(0.0f, 1.0f, 1.0f), t0),
                SR_Color(0.0f, 1.0f, 1.0f),
                SR_Color(1.0f, 0.0f, 1.0f)
            );

            window.fillTriangle(
                SR_Vec2f(tmpIntB.x, tmpIntB.y),
                SR_Vec2f(insB.x, insB.y),
                SR_Vec2f(insA.x, insA.y),
                SR_Color(1.0f, 0.0f, 1.0f),
                SR_Color::lerp(SR_Color(1.0f, 1.0f, 0.0f), SR_Color(1.0f, 0.0f, 1.0f), t1),
                SR_Color::lerp(SR_Color(1.0f, 1.0f, 0.0f), SR_Color(0.0f, 1.0f, 1.0f), t0)
            );

            continue;
        }

        // 剩下的情况就是全部在平面内直接渲染即可

        // 将顶点变换到 NDC
        a = vp * a;
        b = vp * b;
        c = vp * c;

        a.homogenDivide();
        b.homogenDivide();
        c.homogenDivide();

        // window.drawLine(
        //     SR_Vec2f(a.x, a.y),
        //     SR_Vec2f(b.x, b.y),
        //     SR_Color(1.0f)
        // );

        // window.drawLine(
        //     SR_Vec2f(b.x, b.y),
        //     SR_Vec2f(c.x, c.y),
        //     SR_Color(1.0f)
        // );

        // window.drawLine(
        //     SR_Vec2f(c.x, c.y),
        //     SR_Vec2f(a.x, a.y),
        //     SR_Color(1.0f)
        // );

        window.fillTriangle(
            SR_Vec2f(a.x, a.y),
            SR_Vec2f(b.x, b.y),
            SR_Vec2f(c.x, c.y),
            SR_Color(1.0f, 1.0f, 0.0f),
            SR_Color(0.0f, 1.0f, 1.0f),
            SR_Color(1.0f, 0.0f, 1.0f)
        );
    }
}

float insideInPlane(const SR_Vec4f &n, const SR_Vec4f &p)
{
    return n.x * p.x + n.y * p.y + n.z * p.z + n.w * p.w;
}
