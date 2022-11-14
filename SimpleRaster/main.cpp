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

    SR_Mat4x4f vp = SR_Mat4x4f::viewportMatrix(w, h);
    SR_Mat4x4f per = SR_Mat4x4f::perspectiveMatrix(
        SR_Vec3f(-1.0f, -1.0f, -1.0f),
        SR_Vec3f(1.0f, 1.0f, 1.0f)
    );
    SR_Mat4x4f cam = SR_Mat4x4f::cameraMatrix(
        SR_Vec3f(0.0f, 0.0f, 1.5),
        SR_Vec3f(0.0f, 0.0f, 0.0)
    );
    SR_Mat4x4f move = SR_Mat4x4f::translateMatrix(0.0f, 0.0f, 0.0f);
    //move = move * SR_Mat4x4f::rotateXMatrix(PI / 6.0f);

    // 透视变换至齐次裁剪空间
    SR_Mat4x4f proj = per * cam;
    SR_IndexMesh mesh;

    mesh.addVertex(SR_Vec3f(-0.5f, -0.5f, 0.5f), SR_Color(0.0f, 0.0f, 1.0f));
    mesh.addVertex(SR_Vec3f(0.5f, -0.5f, 0.5f), SR_Color(0.0f, 1.0f, 0.0f));
    mesh.addVertex(SR_Vec3f(0.5f, -0.5f, -0.5f), SR_Color(1.0f, 0.0f, 0.0f));
    mesh.addVertex(SR_Vec3f(-0.5f, -0.5f, -0.5f), SR_Color(0.0f, 1.0f, 0.0f));

    mesh.addVertex(SR_Vec3f(-0.5f, -1.0f, 0.0f), SR_Color(0.0f, 1.0f, 1.0f));
    mesh.addVertex(SR_Vec3f(0.5f, -1.0f, 0.0f), SR_Color(0.0f, 1.0f, 1.0f));
    mesh.addVertex(SR_Vec3f(0.0f, 0.0f, 0.0f), SR_Color(0.0f, 1.0f, 1.0f));

    mesh.addIndexList(0, 1, 2);
    mesh.addIndexList(2, 3, 0);
    mesh.addIndexList(4, 5, 6);

    // 定义裁剪网格
    SR_IndexMesh clipMesh = mesh;
    int vertCount = clipMesh.getVertexCount();

    // 先对顶点做透视变换
    for (int i = 0; i < vertCount; i++) {
        SR_VertexInfo vi = clipMesh.getVertexInfo(i);

        // 顶点着色的位置 TODO 
        vi.vertex = proj * move * vi.vertex;
        clipMesh.setVertex(i, vi);
    }

    // 齐次空间裁剪
    // 分别在 near far left right top bottom 平面对顶点进行裁剪
    clipMesh.clipTriangle(SR_Vec4f(0.0f, 0.0f, -1.0f, 1.0f));
    clipMesh.clipTriangle(SR_Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    clipMesh.clipTriangle(SR_Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    clipMesh.clipTriangle(SR_Vec4f(-1.0f, 0.0f, 0.0f, 1.0f));
    clipMesh.clipTriangle(SR_Vec4f(0.0f, -1.0f, 0.0f, 1.0f));
    clipMesh.clipTriangle(SR_Vec4f(0.0f, 1.0f, 0.0f, 1.0f));

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

        // 片元着色、深度测试、光栅化
        // window.fillTriangle(
        //     SR_Vec2f(ia.vertex.x, ia.vertex.y),
        //     SR_Vec2f(ib.vertex.x, ib.vertex.y),
        //     SR_Vec2f(ic.vertex.x, ic.vertex.y),
        //     ia.color,
        //     ib.color,
        //     ic.color
        // );

        window.rasterizeTriangle(ia, ib, ic);
    }
}
