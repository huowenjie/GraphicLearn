#include "srt_window.h"
#include "srt_vec3f.h"

// 逐像素访问，效果类似于片段着色器
static void visitPixels(const SRT_Vec2f &resolution, const SRT_Vec2f &uv, SRT_Color &color);

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
    /**
     * 光线追踪的三步
     * 
     * 1.计算视图射线
     * 2.找到第一个与射线相交的对象并计算其表面法线
     * 3.利用 1、2 获取的信息对像素点着色
     */

    // 将 x 轴的单位进行按比例缩放并将坐标轴向右上移动0.5个单位
    float radio = resolution.x / resolution.y;
    SRT_Vec2f st((uv.x - 0.5f) * radio, uv.y - 0.5f);

    // 定义摄像机位置也是光线的原点
    SRT_Vec3f camera(0.0f, 0.0f, 5.0f);
    SRT_Vec3f imagePlane(st.x, st.y, 0.0f);

    // 计算光线方向
    SRT_Vec3f rayDir = imagePlane - camera;
    rayDir.normalize();

    // 距离场绘制圆
    if (st.length() < 0.2f) {
        color.r = 1.0f;
        color.b = 1.0f;
    }
}
