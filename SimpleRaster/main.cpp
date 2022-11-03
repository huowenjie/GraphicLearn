#include <cmath>
#include <cstdio>

#include "sr_window.h"
#include "sr_vec3f.h"
#include "sr_global.h"
#include "sr_mat2x2f.h"
#include "sr_mat3x3f.h"

// 逐像素访问，效果类似于片段着色器
static void visitPixels(const SR_Vec2f &resolution, const SR_Vec2f &uv, SR_Color &color);

int main(int argc, char *argv[])
{
    // SR_Window window;

    // window.initialize();
    // window.setForeachPixelCallback(visitPixels);
    // window.render();
    // window.terminate();

    SR_Vec3f vec(1.0f, 1.0f, 1.0f);
    SR_Mat3x3f ma = SR_Mat3x3f::unitMatrix();
    SR_Mat3x3f mb = SR_Mat3x3f::zeroMatrix();

    float elems[] = {
        2.0f,  1.0f,  3.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  4.0f, -2.0f
    };

    float elemij[3][3] = {
        { 2.0f,  1.0f,  3.0f },
        { 1.0f, -1.0f,  1.0f },
        { 1.0f,  4.0f, -2.0f }
    };

    SR_Mat3x3f mc(elems);
    SR_Mat3x3f md(elemij);
    SR_Mat3x3f me;
    SR_Mat3x3f mf;
    SR_Mat3x3f mg;
    SR_Mat3x3f mh;
    SR_Mat3x3f mi;

    me = -md;
    mf = me * md;
    mg = mc * mf;
    mh = mg.transpose();

    mi[0][2] = 5.0f;
    mi[1][2] = 5.0f;
    vec = mi * vec;

    ma.printValue("ma");
    mb.printValue("mb");
    mc.printValue("mc");
    md.printValue("md");
    me.printValue("me");
    mf.printValue("mf");
    mg.printValue("mg");
    mh.printValue("mh");
    mi.printValue("mi");
    vec.printValue("vec");

    return 0;
}

void visitPixels(const SR_Vec2f &resolution, const SR_Vec2f &uv, SR_Color &color)
{
    // 将 x 轴的单位进行按比例缩放并将坐标轴向右上移动0.5个单位
    float radio = resolution.x / resolution.y;
    SR_Vec2f st((uv.x - 0.5f) * radio, uv.y - 0.5f);
    color = SR_Color(uv.y);
}
