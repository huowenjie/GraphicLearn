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

    bool initialize();
    bool terminate();

    void render();

public:
    // 设置回调，visitPixel 的参数分别为 x、y、color
    void setForeachPixelCallback(
        void (*visitPixel)(const SR_Vec2f &, const SR_Vec2f &, SR_Color &)
    );

private:
    SR_WinCtx *info;
    int winWidth;
    int winHeight;
};

//-----------------------------------------------------------------------------

#endif // __SR_WINDOW_H__
