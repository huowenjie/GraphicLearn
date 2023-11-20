#include "sr_menu.h"
#include "sr_app.h"
#include "sr_global.h"
#include "sr_window.h"
#include "sr_mat4x4f.h"

//-----------------------------------------------------------------------------

// 主菜单
static char basicMenu[][MENU_STR_LEN] =
{
    "基础渲染测试",
    "1) 点是否在三角形内",
    "2) 三角形交汇",
    "0) 退出"
};

// 判断点是否在三角形内
static void pointInsideTriangle();
static void pitUpdate(SR_Window &window);

// 两个三角形交汇
static void twoTriangle();
static void twoTriangleUpdate(SR_Window &window);

//-----------------------------------------------------------------------------

void renderBasic()
{
    int index = 0;

    while (1) 
    {
        index = SELECT_MENU(basicMenu);
        switch (index) 
        {
        case 1:
            pointInsideTriangle();
            break;

        case 2:
            twoTriangle();
            break;

        case 0:
            return;
        }
    }
}

//-----------------------------------------------------------------------------

void pointInsideTriangle()
{
    SR_Window window;

    window.initialize();
    window.setUpdate(pitUpdate);
    window.render();
    window.terminate();
}

void pitUpdate(SR_Window &window)
{
    float width = (float)window.getWidth();
    float height = (float)window.getHeight();

    SR_Vec2f a(width / 5.0f, height / 5.0f);
    SR_Vec2f b(width * 4.0f / 5.0f, height / 5.0f);
    SR_Vec2f c(width / 2.0f, height * 4.0f / 5.0f);
    SR_Color color(0.0f, 1.0f, 0.0f);

    // 待测样本
    int count = 256;
    SR_Vec2f *tp = new SR_Vec2f[count];

    window.drawLine(a, b, color);
    window.drawLine(b, c, color);
    window.drawLine(c, a, color);

    for (int i = 0; i < count; i++) {
        SR_Vec2f &pt = tp[i];

        pt.x = randomf(1.0f, width - 2.0f);
        pt.y = randomf(1.0f, height - 2.0f);

        // 随机五种颜色
        int colorSel = randomi(1, 5);
        switch(colorSel) {
        case 1: color.setRGB(1.0f, 0.0f, 1.0f); break;
        case 2: color.setRGB(1.0f, 1.0f, 0.0f); break;
        case 3: color.setRGB(0.0f, 1.0f, 1.0f); break;
        case 4: color.setRGB(1.0f, 1.0f, 1.0f); break;
        case 5: color.setRGB(0.0f, 0.0f, 1.0f); break;
        }

        // 如果点在三角形内部，则颜色设置为和三角形一样的颜色
        if (window.insideTriangle(a, b, c, pt)) {
            color.setRGB(0.0f, 1.0f, 0.0f);
        }

        window.drawPixel(pt, color);
    }

    delete[] tp;
}

//-----------------------------------------------------------------------------

void twoTriangle()
{
    SR_Window window;

    window.initialize();
    window.setUpdate(twoTriangleUpdate);
    window.render();
    window.terminate();
}

void twoTriangleUpdate(SR_Window &window)
{
    int w = window.getWidth();
    int h = window.getHeight();
    
    // 摄像机位置
    SR_Vec3f camPos = SR_Vec3f(0.0f, 0.0f, 1.7f);

    // 摄像机注视方向
    SR_Vec3f targetPos = SR_Vec3f(0.0f, 0.0f, 0.0);

    // 构建变换矩阵
    SR_Mat4x4f vp = SR_Mat4x4f::viewportMatrix(w, h);
    SR_Mat4x4f per = SR_Mat4x4f::perspectiveMatrix(
        SR_Vec3f(-1.0f, -1.0f, -1.0f),
        SR_Vec3f(1.0f, 1.0f, 1.0f)
    );
    SR_Mat4x4f cam = SR_Mat4x4f::cameraMatrix(camPos, targetPos);
    SR_Mat4x4f model = SR_Mat4x4f::translateMatrix(0.0f, 0.0f, 0.0f);

    // 透视变换至齐次裁剪空间
    SR_Mat4x4f proj = per * cam;

    SR_IndexMesh mesh;

    // 三个测试三角形
    mesh.addVertex(SR_Vec3f(-0.5f, -0.5f,  0.0f), SR_Color(1.0f, 0.0f, 1.0f));
    mesh.addVertex(SR_Vec3f( 0.5f, -0.5f,  0.0f), SR_Color(1.0f, 0.0f, 1.0f));
    mesh.addVertex(SR_Vec3f( 0.0f,  1.0f,  0.0f), SR_Color(1.0f, 0.0f, 1.0f));

    mesh.addVertex(SR_Vec3f(-1.0f, -1.0f,  0.2f), SR_Color(0.0f, 1.0f, 1.0f));
    mesh.addVertex(SR_Vec3f( 0.5f, -0.2f,  0.2f), SR_Color(0.0f, 1.0f, 1.0f));
    mesh.addVertex(SR_Vec3f( 1.0f,  1.0f, -0.2f), SR_Color(0.0f, 1.0f, 1.0f));

    mesh.addVertex(SR_Vec3f(-0.5f,  0.5f,  0.5f), SR_Color(0.5f, 0.5f, 0.3f));
    mesh.addVertex(SR_Vec3f( 1.0f,  0.5f,  0.5f), SR_Color(0.5f, 0.5f, 0.3f));
    mesh.addVertex(SR_Vec3f(-0.2f,  0.9f, -0.2f), SR_Color(0.5f, 0.5f, 0.3f));

    mesh.addIndexList(0, 1, 2);
    mesh.addIndexList(3, 4, 5);
    mesh.addIndexList(6, 7, 8);

    int num = mesh.getTriangleCount();

    // 计算法线向量
    for (int i = 0; i < num; i++) {
        SR_TriangleIndexList list = mesh.getIndexList(i);
        SR_VertexInfo ia = mesh.getVertexInfo(list.indexList[0]);
        SR_VertexInfo ib = mesh.getVertexInfo(list.indexList[1]);
        SR_VertexInfo ic = mesh.getVertexInfo(list.indexList[2]);

        SR_Vec4f hba = ia.vertex - ib.vertex;
        SR_Vec4f hbc = ic.vertex - ib.vertex;

        // 三角形两边叉乘求法向量
        SR_Vec3f ba = SR_Vec3f(hba.x, hba.y, hba.z);
        SR_Vec3f bc = SR_Vec3f(hbc.x, hbc.y, hbc.z);
        SR_Vec3f n = bc.cross(ba);
        n.normalize();

        // 同时将法线向量做模型变换
        SR_Vec4f tmp = SR_Mat4x4f::transpose(SR_Mat4x4f::inverse(model)) * SR_Vec4f(n, 1.0f);
        list.normal = SR_Vec3f(tmp.x, tmp.y, tmp.z);

        mesh.setIndexList(i, list);
    }

    // 定义裁剪网格
    SR_IndexMesh clipMesh = mesh;
    int vertCount = clipMesh.getVertexCount();

    // 先对顶点做透视变换
    for (int i = 0; i < vertCount; i++) {
        SR_VertexInfo vi = clipMesh.getVertexInfo(i);

        // 对顶点进行模型变换然后记录顶点在世界坐标系的位置
        SR_Vec4f global = model * vi.vertex;
        vi.global = SR_Vec3f(global.x, global.y, global.z);

        // 透视变换
        vi.vertex = proj * model * vi.vertex;
        clipMesh.setVertex(i, vi);
    }

    // 在齐次坐标空间裁剪
    // 分别在 near far left right top bottom 平面对顶点进行裁剪
    clipMesh.clipTriangle(SR_Vec4f(0.0f, 0.0f, -1.0f, 1.0f));
    clipMesh.clipTriangle(SR_Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    clipMesh.clipTriangle(SR_Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    clipMesh.clipTriangle(SR_Vec4f(-1.0f, 0.0f, 0.0f, 1.0f));
    clipMesh.clipTriangle(SR_Vec4f(0.0f, -1.0f, 0.0f, 1.0f));
    clipMesh.clipTriangle(SR_Vec4f(0.0f, 1.0f, 0.0f, 1.0f));

    num = clipMesh.getTriangleCount();

    for (int i = 0; i < num; i++) {
        const SR_TriangleIndexList &list = clipMesh.getIndexList(i);

        SR_VertexInfo ia = clipMesh.getVertexInfo(list.indexList[0]);
        SR_VertexInfo ib = clipMesh.getVertexInfo(list.indexList[1]);
        SR_VertexInfo ic = clipMesh.getVertexInfo(list.indexList[2]);

        // 透视除法
        ia.vertex.homogenDivide();
        ib.vertex.homogenDivide();
        ic.vertex.homogenDivide();

        // 将顶点变换到屏幕空间
        ia.vertex = vp * ia.vertex;
        ib.vertex = vp * ib.vertex;
        ic.vertex = vp * ic.vertex;

        // 片元着色、深度测试、光栅化
        window.rasterizeTriangle(list, ia, ib, ic, nullptr);
    }
}

//-----------------------------------------------------------------------------
