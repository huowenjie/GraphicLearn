#ifndef __SR_WINDOW_H__
#define __SR_WINDOW_H__

#include "sr_fragment.h"

//-----------------------------------------------------------------------------
// 窗口上下文
//-----------------------------------------------------------------------------

// 窗口信息
class SR_WinCtx;

// 窗口上下文
class SR_Window
{
public:
    SR_Window(int width = 480, int height = 360);
    ~SR_Window();

public:

    bool initialize();
    bool terminate();

    void setStart(void (*start)(SR_Window &));
    void setUpdate(void (*update)(SR_Window &));

    // 绘制像素
    void drawPixel(const SR_Vec2f &pos, const SR_Color &color);

    // 画线
    void drawLine(const SR_Vec2f &a, const SR_Vec2f &b, const SR_Color &color);

    // 通过三角形重心坐标对三角形进行插值
    void rasterizeTriangle(
        const SR_TriangleIndexList &list,
        const SR_VertexInfo &a,
        const SR_VertexInfo &b,
        const SR_VertexInfo &c,
        SR_Color (*fragmentShader)(const SR_Fragment &)
    );

    // 深度测试，如果 depth 深度大于 pos 位置原有的深度则返回 true，否则返回 false
    bool zbufferTest(const SR_Vec2f &pos, float depth);

    //  TODO 判断点 p 是否在三角形 abc 内
    bool insideTriangle(
        const SR_Vec2f &a,
        const SR_Vec2f &b,
        const SR_Vec2f &c,
        const SR_Vec2f &p
    );

    // 帧渲染
    void render();

public:
    int getWidth() const;
    int getHeight() const;

private:
    SR_WinCtx *info;
    int winWidth;
    int winHeight;

    void (*start)(SR_Window &);
    void (*update)(SR_Window &);
};

//-----------------------------------------------------------------------------

#endif // __SR_WINDOW_H__
