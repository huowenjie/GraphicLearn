#ifndef __SR_WINDOW_H__
#define __SR_WINDOW_H__

#include "sr_color.h"
#include "sr_vec2f.h"

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

    void setUpdate(void (*update)(SR_Window &));

    // 绘制像素
    void drawPixel(const SR_Vec2f &pos, const SR_Color &color);

    // 画线
    void drawLine(const SR_Vec2f &a, const SR_Vec2f &b, const SR_Color &color);

    // 帧渲染
    void render();

public:
    int getWidth() const;
    int getHeight() const;

private:
    SR_WinCtx *info;
    int winWidth;
    int winHeight;

    void (*update)(SR_Window &);
};

//-----------------------------------------------------------------------------

#endif // __SR_WINDOW_H__
