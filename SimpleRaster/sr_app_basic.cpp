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
    "3) 片元着色器绘制 Piet Mondria",
    "0) 退出"
};

// 判断点是否在三角形内
static void pointInsideTriangle();
static void pitUpdate(SR_Window &window);

// 两个三角形交汇
static void twoTriangle();
static void twoTriangleUpdate(SR_Window &window);

// 绘制 Piet Mondria
static void drawPietMondria();
static void drawPietMondria(SR_Window &window);

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

        case 3:
            drawPietMondria();
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
    SR_Vec3f camPos = SR_Vec3f(0.0f, 0.0f, 2.0f);

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
    clipMesh.clipTriangle();
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

/*
 * 绘制矩形
 *
 * st 当前 frag 归一化坐标
 * l 矩形长度
 * h 矩形高度
 * center 矩形中心点
 */
float drawRect(const SR_Vec2f &st, float l, float h, const SR_Vec2f &center) {
    float tl = (1.0 - l) * 0.5;
    float th = (1.0 - h) * 0.5;
    SR_Vec2f c = SR_Vec2f(center.x - 0.5, 1.0 - center.y - 0.5);

    SR_Vec2f lb = SR_Vec2f::step(SR_Vec2f(tl + c.x, th + c.y), st);
    SR_Vec2f rt = SR_Vec2f::step(st, SR_Vec2f(1.0 - tl + c.x, 1.0 - th + c.y));

    return lb.x * lb.y * rt.x* rt.y;
}

SR_Color pietMondriaShader(const SR_Fragment &frag)
{
    SR_Color red = SR_Color(0.71f, 0.14f, 0.15f);
    SR_Color yellow = SR_Color(0.99f, 0.77f, 0.2f);
    SR_Color blue = SR_Color(0.0f, 0.36f, 0.6f);
    SR_Color black = SR_Color(0.12f, 0.14f, 0.15f);

    SR_Vec3f coord = frag.getFragCoord();
    SR_Vec3f resolution = frag.getResolution();
    SR_Vec2f st = SR_Vec2f(coord.x / resolution.x, coord.y / resolution.y);

    SR_Color back = SR_Color(0.96f, 0.93f, 0.87f);

    // 先混合背景色和主色块
    back = SR_Color::lerp(back, red, drawRect(st, 0.2f, 0.4f, SR_Vec2f(0.1f, 0.2f)));
    back = SR_Color::lerp(back, yellow, drawRect(st, 0.05f, 0.4f, SR_Vec2f(0.975f, 0.2f)));
    back = SR_Color::lerp(back, blue, drawRect(st, 0.3f, 0.1f, SR_Vec2f(0.85f, 0.95f)));

    // 接下来是几个覆盖的黑色线条 横向
    back = SR_Color::lerp(back, black, drawRect(st, 1.0f, 0.035f, SR_Vec2f(0.5f, 0.2f)));
    back = SR_Color::lerp(back, black, drawRect(st, 1.0f, 0.035f, SR_Vec2f(0.5f, 0.4f)));
    back = SR_Color::lerp(back, black, drawRect(st, 0.8f, 0.03f, SR_Vec2f(0.6f, 0.9f)));

    // 纵向
    back = SR_Color::lerp(back, black, drawRect(st, 0.025f, 0.4f, SR_Vec2f(0.06f, 0.2f)));
    back = SR_Color::lerp(back, black, drawRect(st, 0.03f, 1.0f, SR_Vec2f(0.2f, 0.5f)));
    back = SR_Color::lerp(back, black, drawRect(st, 0.03f, 1.0f, SR_Vec2f(0.7f, 0.5f)));
    back = SR_Color::lerp(back, black, drawRect(st, 0.03f, 1.0f, SR_Vec2f(0.95f, 0.5f)));
    return back;
}

void drawPietMondria()
{
    SR_Window window;

    window.initialize();
    window.setUpdate(drawPietMondria);
    window.render();
    window.terminate();
}

void drawPietMondria(SR_Window &window)
{
    int w = window.getWidth();
    int h = window.getHeight();

    // 摄像机位置
    SR_Vec3f camPos = SR_Vec3f(0.0f, 0.0f, 1.0f);

    // 摄像机注视方向
    SR_Vec3f targetPos = SR_Vec3f(0.0f, 0.0f, 0.0);

    // 构建变换矩阵
    SR_Mat4x4f vp = SR_Mat4x4f::viewportMatrix(w, h);
    SR_Mat4x4f ortho = SR_Mat4x4f::orthoMatrix(
        SR_Vec3f(-1.0f, -1.0f, -1.0f),
        SR_Vec3f(1.0f, 1.0f, 1.0f)
    );

    SR_Mat4x4f cam = SR_Mat4x4f::cameraMatrix(camPos, targetPos);
    SR_Mat4x4f model = SR_Mat4x4f::translateMatrix(0.0f, 0.0f, 0.0f);

    // 透视变换至齐次裁剪空间
    SR_Mat4x4f proj = ortho * cam;

    SR_IndexMesh mesh;

    // 两个个测试三角形
    mesh.addVertex(SR_Vec3f(-1.0f, -1.0f,  0.0f), SR_Color(1.0f, 1.0f, 1.0f));
    mesh.addVertex(SR_Vec3f( 1.0f, -1.0f,  0.0f), SR_Color(1.0f, 1.0f, 1.0f));
    mesh.addVertex(SR_Vec3f( 1.0f,  1.0f,  0.0f), SR_Color(1.0f, 1.0f, 1.0f));
    mesh.addVertex(SR_Vec3f(-1.0f,  1.0f,  0.0f), SR_Color(1.0f, 1.0f, 1.0f));

    mesh.addIndexList(0, 1, 2);
    mesh.addIndexList(2, 3, 0);

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

    clipMesh.clipTriangle();
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
        window.rasterizeTriangle(list, ia, ib, ic, pietMondriaShader);
    }
}

//-----------------------------------------------------------------------------
