#include <cmath>
#include <cstdio>

#include "sr_window.h"
#include "sr_vec3f.h"
#include "sr_global.h"
#include "sr_mat2x2f.h"

// 逐像素访问，效果类似于片段着色器
static void visitPixels(const SR_Vec2f &resolution, const SR_Vec2f &uv, SR_Color &color);

int main(int argc, char *argv[])
{
    // SR_Window window;

    // window.initialize();
    // window.setForeachPixelCallback(visitPixels);
    // window.render();
    // window.terminate();

    SR_Vec2f vec(1.0f, 1.0f);
    SR_Mat2x2f ma = SR_Mat2x2f::unitMatrix();
    SR_Mat2x2f mb = SR_Mat2x2f::zeroMatrix();

    float elems[] = { 1.0f, 2.0f, 3.0f, 4.0f };
    float elemij[2][2] = {
        { 1.0f, 2.0f },
        { 3.0f, 4.0f }
    };

    SR_Mat2x2f mc(elems);
    SR_Mat2x2f md(elemij);
    SR_Mat2x2f me;
    SR_Mat2x2f mf;
    SR_Mat2x2f mg;
    SR_Mat2x2f mh;
    SR_Mat2x2f mi;

    me = mc * md * ma;
    mf = me.transpose();
    mg = 10.0f * (me - mf) * 10.0f;

    ma.printValue("ma");
    mb.printValue("mb");
    mc.printValue("mc");
    md.printValue("md");
    me.printValue("me");
    mf.printValue("mf");
    // vec.printValue("vec");

    mh = -mf;
    mi = mh * mf;

    mg.printValue("mg");
    mh.printValue("mh");
    mi.printValue("mi");

    return 0;
}

void visitPixels(const SR_Vec2f &resolution, const SR_Vec2f &uv, SR_Color &color)
{
    // 将 x 轴的单位进行按比例缩放并将坐标轴向右上移动0.5个单位
    float radio = resolution.x / resolution.y;
    SR_Vec2f st((uv.x - 0.5f) * radio, uv.y - 0.5f);
    color = SR_Color(uv.y);
}
