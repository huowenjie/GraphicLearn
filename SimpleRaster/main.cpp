#include <cmath>
#include <cstdio>

#include "sr_window.h"
#include "sr_mat4x4f.h"
#include "sr_global.h"

// 逐像素访问，效果类似于片段着色器
static void visitPixels(const SR_Vec2f &resolution, const SR_Vec2f &uv, SR_Color &color);

int main(int argc, char *argv[])
{
    // SR_Window window;

    // window.initialize();
    // window.setForeachPixelCallback(visitPixels);
    // window.render();
    // window.terminate();

    return 0;
}

void visitPixels(const SR_Vec2f &resolution, const SR_Vec2f &uv, SR_Color &color)
{
    // 将 x 轴的单位进行按比例缩放并将坐标轴向右上移动0.5个单位
    float radio = resolution.x / resolution.y;
    SR_Vec2f st((uv.x - 0.5f) * radio, uv.y - 0.5f);
    color = SR_Color(uv.y);
}
