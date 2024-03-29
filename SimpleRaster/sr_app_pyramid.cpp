#include <cmath>
#include <cstdio>

#include "sr_mat4x4f.h"
#include "sr_global.h"
#include "sr_app.h"

#include "sr_window.h"

//-----------------------------------------------------------------------------

static void update(SR_Window &window);
static void start(SR_Window &window);

// 片段着色
static SR_Color fragmentShader(const SR_Fragment &frag);

//-----------------------------------------------------------------------------

void renderPyramid()
{
    SR_Window window;

    window.initialize();

    window.setStart(start);
    window.setUpdate(update);
    window.render();

    window.terminate();
}

// 光源位置
static SR_Vec3f lightPos = SR_Vec3f(0.0f, 0.0f, 2.0f);

// 光线颜色
static SR_Color lightColor = SR_Color(1.0f);

// 摄像机位置
static SR_Vec3f camPos = SR_Vec3f(0.0f, 0.0f, 2.0f);

// 摄像机注视方向
static SR_Vec3f targetPos = SR_Vec3f(0.0f, 0.0f, 0.0);

// 初始旋转角度
static float startRotate = (PI / 180.0f) * 44.5f;
static float rotateSpeed = (PI / 360.0f);

void start(SR_Window &window)
{
}

void update(SR_Window &window)
{
    int w = window.getWidth();
    int h = window.getHeight();

    // 构建转换矩阵
    SR_Mat4x4f vp = SR_Mat4x4f::viewportMatrix(w, h);
    SR_Mat4x4f per = SR_Mat4x4f::perspectiveMatrix(
        SR_Vec3f(-1.0f, -1.0f, -1.0f),
        SR_Vec3f(1.0f, 1.0f, 1.0f)
    );
    SR_Mat4x4f cam = SR_Mat4x4f::cameraMatrix(camPos, targetPos);
    SR_Mat4x4f model = SR_Mat4x4f::translateMatrix(0.0f, 0.0f, 0.0f);

    startRotate += rotateSpeed;
    model = model * SR_Mat4x4f::rotateYMatrix(startRotate);

    SR_IndexMesh mesh;

    // 一个测试四棱锥
    mesh.addVertex(SR_Vec3f(-0.5f,  0.0f,  0.5f), SR_Color(0.1f, 0.5f, 0.3f));
    mesh.addVertex(SR_Vec3f( 0.5f,  0.0f,  0.5f), SR_Color(0.1f, 0.5f, 0.3f));
    mesh.addVertex(SR_Vec3f( 0.5f,  0.0f, -0.5f), SR_Color(0.1f, 0.5f, 0.3f));
    mesh.addVertex(SR_Vec3f(-0.5f,  0.0f, -0.5f), SR_Color(0.1f, 0.5f, 0.3f));
    mesh.addVertex(SR_Vec3f( 0.0f,  1.0f,  0.0f), SR_Color(0.1f, 0.5f, 0.3f));
    mesh.addVertex(SR_Vec3f( 0.0f, -1.0f,  0.0f), SR_Color(0.1f, 0.5f, 0.3f));

    mesh.addIndexList(0, 1, 4);
    mesh.addIndexList(1, 2, 4);
    mesh.addIndexList(2, 3, 4);
    mesh.addIndexList(3, 0, 4);

    mesh.addIndexList(0, 5, 1);
    mesh.addIndexList(1, 5, 2);
    mesh.addIndexList(2, 5, 3);
    mesh.addIndexList(3, 5, 0);

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

        // 对顶点进行模型变换然后记录顶点在相机坐标系的位置
        SR_Vec4f global = model * vi.vertex;
        vi.global = SR_Vec3f(global.x, global.y, global.z);

        // 透视变换
        vi.vertex = per * cam * model * vi.vertex;
        clipMesh.setVertex(i, vi);
    }

    clipMesh.clipTriangle();
    num = clipMesh.getTriangleCount();

    for (int i = 0; i < num; i++) {
        const SR_TriangleIndexList &list = clipMesh.getIndexList(i);

        SR_VertexInfo ia = clipMesh.getVertexInfo(list.indexList[0]);
        SR_VertexInfo ib = clipMesh.getVertexInfo(list.indexList[1]);
        SR_VertexInfo ic = clipMesh.getVertexInfo(list.indexList[2]);

        // 将顶点变换到屏幕空间
        ia.vertex = vp * ia.vertex;
        ib.vertex = vp * ib.vertex;
        ic.vertex = vp * ic.vertex;

        // 透视除法
        ia.vertex.homogenDivide();
        ib.vertex.homogenDivide();
        ic.vertex.homogenDivide();

        // 片元着色、深度测试、光栅化
        window.rasterizeTriangle(list, ia, ib, ic, fragmentShader);
    }
}

SR_Color fragmentShader(const SR_Fragment &frag)
{
    // phong 着色

    // 反射颜色
    SR_Color cr = frag.getFragColor();

    // 获取片元在世界坐标的位置
    SR_Vec3f position = frag.getFragGlobal();

    // 获取光照方向
    SR_Vec3f lightDir = lightPos - position;
    lightDir.normalize();

    // 获取面法线向量
    SR_Vec3f normal = frag.getFragSurfaceNormal();

    // 计算漫反射光
    float diff = srMaxf(0.0f, lightDir.dot(normal));
    SR_Color diffuse = diff * lightColor;

    // 计算环境光
    SR_Color ambient = 0.3f * lightColor;
#if 1
    // 计算反射光线 r = −l + 2(l · n)n
    SR_Vec3f r = -lightDir + 2.0f * (normal.dot(lightDir)) * normal;
    r.normalize();

    SR_Vec3f e = camPos - position;
    e.normalize();

    // 计算高光 max(0, e · r) ^ p
    SR_Color highLight = lightColor * std::pow(srMaxf(0.0f, e.dot(r)), 3.0f);
#else
    // 简便方法计算高光 h = (e + l) / |e + l|
    SR_Vec3f e = camPos - frag.vertex.global;
    e.normalize();
    SR_Vec3f h = e + lightDir;
    h.normalize();

    // 计算高光 (h · n) ^ p
    SR_Color highLight = lightColor * std::pow(h.dot(normal), 16.0f);
#endif

    SR_Color fragColor = (diffuse + ambient) * cr + highLight;
    fragColor.clamp();
    return fragColor;
}

//-----------------------------------------------------------------------------
