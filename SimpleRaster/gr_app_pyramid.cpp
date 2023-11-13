#include <cmath>
#include <cstdio>

#include "sr_window.h"
#include "sr_mat4x4f.h"
#include "sr_global.h"
#include "sr_fragment.h"

#include "gr_app.h"

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
static SR_Vec3f lightPos = SR_Vec3f(2.0f, 2.0f, 0.0f);

// 光线颜色
static SR_Color lightColor = SR_Color(1.0f);

// 摄像机位置
static SR_Vec3f camPos = SR_Vec3f(0.0f, 0.0f, 1.7f);

// 摄像机注视方向
static SR_Vec3f targetPos = SR_Vec3f(0.0f, 0.0f, 0.0);

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
    model = model * SR_Mat4x4f::rotateYMatrix(-PI / 3.0f);

    // 透视变换至齐次裁剪空间
    SR_Mat4x4f proj = per * cam;

    SR_IndexMesh mesh;

    // 一个测试四棱锥
    mesh.addVertex(SR_Vec3f(0.0f, -1.0f, 0.0f), SR_Color(0.0f, 0.0f, 1.0f));
    mesh.addVertex(SR_Vec3f(-0.5f, 0.0f, 0.5f), SR_Color(1.0f, 1.0f, 0.0f));
    mesh.addVertex(SR_Vec3f(0.5f, 0.0f, 0.5f), SR_Color(1.0f, 0.0f, 1.0f));
    mesh.addVertex(SR_Vec3f(0.5f, 0.0f, -0.5f), SR_Color(1.0f, 0.0f, 1.0f));
    mesh.addVertex(SR_Vec3f(-0.5f, 0.0f, -0.5f), SR_Color(1.0f, 1.0f, 0.0f));
    mesh.addVertex(SR_Vec3f(0.0f, 1.0f, 0.0f), SR_Color(0.0f, 0.0f, 1.0f));

    mesh.addIndexList(0, 2, 1);
    mesh.addIndexList(0, 3, 2);
    mesh.addIndexList(0, 4, 3);
    mesh.addIndexList(0, 1, 4);

    mesh.addIndexList(5, 1, 2);
    mesh.addIndexList(5, 2, 3);
    mesh.addIndexList(5, 3, 4);
    mesh.addIndexList(5, 4, 1);

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
        window.rasterizeTriangle(list, ia, ib, ic, fragmentShader);
    }
}

SR_Color fragmentShader(const SR_Fragment &frag)
{
    // phong 着色
    
    // 反射颜色
    SR_Color cr = frag.vertex.color;

    // 获取光照方向
    SR_Vec3f lightDir = lightPos - frag.vertex.global;
    lightDir.normalize();
    SR_Vec3f normal = frag.normal;

    // 计算漫反射光
    float diff = srMaxf(0.0f, lightDir.dot(normal));
    SR_Color diffuse = diff * lightColor;

    // 计算环境光
    SR_Color ambient = 0.3f * lightColor;
#if 1
    // 计算反射光线 r = −l + 2(l · n)n
    SR_Vec3f r = -lightDir + 2.0f * (normal.dot(lightDir)) * normal;
    r.normalize();

    SR_Vec3f e = camPos - frag.vertex.global;
    e.normalize();

    // 计算高光 max(0, e · r) ^ p
    SR_Color highLight = lightColor * std::pow(srMaxf(0.0f, e.dot(r)), 32.0f);
#else
    // 简便方法计算高光 h = (e + l) / |e + l|
    SR_Vec3f e = camPos - frag.vertex.global;
    e.normalize();
    SR_Vec3f h = e + lightDir;
    h.normalize();

    // 计算高光 (h · n) ^ p
    SR_Color highLight = lightColor * std::pow(h.dot(normal), 64.0f);
#endif

    SR_Color fragColor = (diffuse + ambient) * cr + 0.6f * highLight;
    fragColor.clamp();

    return fragColor;
}

//-----------------------------------------------------------------------------
