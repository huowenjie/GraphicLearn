#ifndef __SRT_WINDOW_H__
#define __SRT_WINDOW_H__

#include "srt_color.h"
#include "srt_vec2f.h"

//-----------------------------------------------------------------------------
// 窗口上下文
//-----------------------------------------------------------------------------

// 窗口信息
class SRT_WinCtx;

// 窗口上下文
class SRT_Window
{
public:
    SRT_Window(int width = 480, int height = 360);
    ~SRT_Window();

    bool initialize();
    bool terminate();

    void render();

public:
    // 设置回调，visitPixel 的参数分别为 x、y、color
    void setForeachPixelCallback(
        void (*visitPixel)(const SRT_Vec2f &, const SRT_Vec2f &, SRT_Color &)
    );

private:
    SRT_WinCtx *info;
    int winWidth;
    int winHeight;
};

//-----------------------------------------------------------------------------

#endif // __SRT_WINDOW_H__
