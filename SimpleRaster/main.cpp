#include <cmath>
#include <cstdio>

#include "sr_window.h"
#include "sr_mat4x4f.h"
#include "sr_global.h"
#include "sr_indexmesh.h"

static void update(SR_Window &window);

// 判断顶点是否在平面内，n 为平面的法线
static bool insideInPlane(const SR_Vec4f &n, float &d, const SR_Vec4f &p);

int main(int argc, char *argv[])
{
    SR_Window window;

    window.initialize();

    window.setUpdate(update);
    window.render();

    window.terminate();

    return 0;
}

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
        SR_Vec4f(0.0f, 0.0f, distance, 1.0f),
        SR_Vec4f(0.0f, 0.0f, target, 1.0f)
    );

    // if (distance > 0.5f) {
    //     distance -= 0.02f;
    //     target -= 0.02f;
    // }

    // 透视变换至齐次裁剪空间
    SR_Mat4x4f proj = per * cam;

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
     * 齐次裁剪空间的定义：
     * -x + lw = 0,
     *  x - rw = 0,
     * -y + bw = 0,
     *  y - tw = 0,
     * -z + nw = 0,
     *  z - fw = 0
     * 
     * 规范化后有
     * 
     * -x - w = 0, =>  x + w = 0
     *  x - w = 0, => -x + w = 0
     * -y - w = 0, =>  y + w = 0
     *  y - w = 0, => -y + w = 0
     * -z - w = 0, =>  z + w = 0
     *  z - w = 0  => -z + w = 0
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

    SR_IndexMesh mesh;

    mesh.addVertex(SR_Vec3f(-0.5f, -0.5f, 0.9f));
    mesh.addVertex(SR_Vec3f(0.5f, -0.5f, 0.5f));
    mesh.addVertex(SR_Vec3f(0.5f, -0.5f, -0.5f));
    mesh.addVertex(SR_Vec3f(-0.5f, -0.5f, -0.5f));
    mesh.addIndexList(0, 1, 2);
    mesh.addIndexList(2, 3, 0);

    for (int i = 0; i < 2; i++) {
        const SR_TriangleIndexList &list = mesh.getIndexList(i);

        SR_Vec4f a = SR_Vec4f(mesh.getVertex(list.indexList[0]), 1.0f);
        SR_Vec4f b = SR_Vec4f(mesh.getVertex(list.indexList[1]), 1.0f);
        SR_Vec4f c = SR_Vec4f(mesh.getVertex(list.indexList[2]), 1.0f);

        /**
         * 这里先试着求三角形 near 的交点
         * 
         * near 的 n 为 (0.0f, 0.0f, -1.0f)
         * near 的 q 为 (0.0f, 0.0f, 1.0f)
         * near 的 D 为 1.0f
         * 
         * 对于一个三角形的处理流程
         * 
         * 1.首先判断三个点是否都在平面内
         * 2.判断三个点是否都在平面外
         * 3.判断
         * f(a)
         * f(b)
         */

        // 先将顶点变换到 NDC
        a = proj * a;
        b = proj * b;
        c = proj * c;

        float d = 0.0f;

        if (insideInPlane(SR_Vec4f(0.0f, 0.0f, -1.0f, 1.0f), d, a)) {
            std::printf("a[%d] 点在内侧 d = %f\n", i, d);
        } else {
            std::printf("a[%d] 点在外侧 d = %f\n", i, d);
        }

        a.printValue("a");

        SR_Vec4f t = a;
        t.homogeneous();
        t.printValue("ha");

        a = vp * a;
        b = vp * b;
        c = vp * c;

        a.homogeneous();
        b.homogeneous();
        c.homogeneous();

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

bool insideInPlane(const SR_Vec4f &n, float &d, const SR_Vec4f &p)
{
    // SR_Vec4f pq = p - q;
    // pq.w = p.w - q.w;

    d = n.dot(p) + n.w * p.w;
    return d <= 0.0f;
}
