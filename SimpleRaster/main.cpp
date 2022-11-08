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
        SR_Vec4f(0.0f, 0.0f, 1.5f, 1.0f),
        SR_Vec4f(0.0f, 0.0f, 0.0f, 1.0f)
    );

    // 透视变换
    SR_Mat4x4f proj = vp * per * cam;
    SR_IndexMesh mesh;

    mesh.addVertex(SR_Vec3f(-0.5f, -0.5f, 0.5f));
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

        a = proj * a;
        b = proj * b;
        c = proj * c;

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
            SR_Color(1.0f, 0.0f, 0.0f),
            SR_Color(0.0f, 1.0f, 0.0f),
            SR_Color(0.0f, 0.0f, 1.0f)
        );
    }
}
