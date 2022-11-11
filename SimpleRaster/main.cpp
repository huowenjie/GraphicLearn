#include <cmath>
#include <cstdio>

#include "sr_window.h"
#include "sr_mat4x4f.h"
#include "sr_global.h"
#include "sr_indexmesh.h"

static void update(SR_Window &window);

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

    static float distance = 1.8f;
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
    SR_Mat4x4f move = SR_Mat4x4f::translateMatrix(-0.5f, 0.0f, 0.5f);
    move = move * SR_Mat4x4f::rotateYMatrix(PI / 4.0f);

    // if (distance > 1.0f) {
    //     distance -= 0.02f;
    //     target -= 0.02f;
    // }

    // 透视变换至齐次裁剪空间
    SR_Mat4x4f proj = per * cam;
    SR_IndexMesh mesh;

    mesh.addVertex(SR_Vec3f(-0.5f, -0.5f, 0.5f), SR_Color(0.0f, 0.0f, 1.0f));
    mesh.addVertex(SR_Vec3f(0.5f, -0.5f, 0.5f), SR_Color(0.0f, 1.0f, 0.0f));
    mesh.addVertex(SR_Vec3f(0.5f, -0.5f, -0.5f), SR_Color(1.0f, 0.0f, 0.0f));
    mesh.addVertex(SR_Vec3f(-0.5f, -0.5f, -0.5f), SR_Color(0.0f, 1.0f, 0.0f));
    mesh.addIndexList(0, 1, 2);
    mesh.addIndexList(2, 3, 0);

    float clipFactor = distance - 1.0f;

    // 近平面裁剪辅助线
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

    // 定义裁剪网格
    SR_IndexMesh clipMesh = mesh;
    int vertCount = clipMesh.getVertexCount();

    // 先对顶点做透视变换
    for (int i = 0; i < vertCount; i++) {
        SR_VertexInfo vi = clipMesh.getVertexInfo(i);
        vi.vertex = proj * move * vi.vertex;
        clipMesh.setVertex(i, vi);
    }

    // 齐次空间裁剪
    //clipMesh.clipTriangle(SR_Vec4f(0.0f, 0.0f, -1.0f, 1.0f));
    clipMesh.clipTriangle(SR_Vec4f(-1.0f, 0.0f, 0.0f, 1.0f));

    // NDC 变换
    int num = clipMesh.getTriangleCount();

    for (int i = 0; i < num; i++) {
        const SR_TriangleIndexList &list = clipMesh.getIndexList(i);

        SR_VertexInfo ia = clipMesh.getVertexInfo(list.indexList[0]);
        SR_VertexInfo ib = clipMesh.getVertexInfo(list.indexList[1]);
        SR_VertexInfo ic = clipMesh.getVertexInfo(list.indexList[2]);

        // 将顶点变换到 NDC
        ia.vertex = vp * ia.vertex;
        ib.vertex = vp * ib.vertex;
        ic.vertex = vp * ic.vertex;

        // 透视除法
        ia.vertex.homogenDivide();
        ib.vertex.homogenDivide();
        ic.vertex.homogenDivide();

        // 渲染三角形
        window.fillTriangle(
            SR_Vec2f(ia.vertex.x, ia.vertex.y),
            SR_Vec2f(ib.vertex.x, ib.vertex.y),
            SR_Vec2f(ic.vertex.x, ic.vertex.y),
            ia.color,
            ib.color,
            ic.color
        );
    }
}
