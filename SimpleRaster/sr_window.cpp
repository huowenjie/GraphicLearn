#include <SDL2/SDL.h>
#include <cstring>
#include <cstdio>
#include "sr_window.h"

//-----------------------------------------------------------------------------

// 窗口信息
class SR_WinCtx
{
public:
    SR_WinCtx()
    {
        window = nullptr;
        texture = nullptr;
        renderer = nullptr;

        surfaceBuffer = nullptr;
    }
public:
    SDL_Window *window;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    SDL_Event evt;

    // 帧缓冲
    Uint32 *surfaceBuffer;
};

SR_Window::SR_Window(int width, int height)
{
    info = new SR_WinCtx();
    winWidth = width;
    winHeight = height;

    Uint32 *buffer = new Uint32[width * height];
    std::memset(buffer, 0, width * height * sizeof(Uint32));

    info->surfaceBuffer = buffer;
    this->update = nullptr;
}

SR_Window::~SR_Window()
{
    if (info) {
        if (info->surfaceBuffer) {
            delete info->surfaceBuffer;
            info->surfaceBuffer = nullptr;
        }

        delete info;
        info = nullptr;
    }
}

bool SR_Window::initialize()
{    
    SDL_Window *window = nullptr;
    SDL_Texture *texture = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (!info) {
        return false;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}

    window = SDL_CreateWindow(
        "SDL2 Pixel Drawing",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        winWidth,
        winHeight,
        0
    );

    if (!window) {
        goto err;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        goto err;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        winWidth,
        winHeight
    );

    if (!texture) {
        goto err;
    }

    info->window = window;
    info->texture = texture;
    info->renderer = renderer;

    return true;

err:
    if (texture) {
        SDL_DestroyTexture(texture);
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
    return false;
}

bool SR_Window::terminate()
{
    if (!info) {
        return false;
    }

    if (info->texture) {
        SDL_DestroyTexture(info->texture);
    }

    if (info->renderer) {
        SDL_DestroyRenderer(info->renderer);
    }

    if (info->window) {
        SDL_DestroyWindow(info->window);
    }

    SDL_Quit();
    return true;
}

void SR_Window::setUpdate(void (*update)(SR_Window &))
{
    this->update = update;
}

void SR_Window::drawPixel(const SR_Vec2f &pos, const SR_Color &color)
{
    int x = pos.x;
    int y = pos.y;
    Uint32 *surfaceBuffer = info->surfaceBuffer;

    if (x > winWidth - 1 || x < 0) {
        return;
    }

    if (y > winHeight - 1 || y < 0) {
        return;
    }

    y = winHeight - 1 - y;

    // 点光栅化
    surfaceBuffer[y * winWidth + x] = color.toUintRGB();
}

void SR_Window::drawLine(const SR_Vec2f &a, const SR_Vec2f &b, const SR_Color &color)
{
    int x0 = a.x;
    int y0 = a.y;
    int x1 = b.x;
    int y1 = b.y;

    int dx = 0;
    int dy = 0;

    int i = x0;
    int j = y0;

    float d = 0.0f;

    if (x0 > winWidth - 1 || x0 < 0 ||
        x1 > winWidth - 1 || x1 < 0 ) {
        return;
    }

    if (y0 > winHeight - 1 || y0 < 0 ||
        y1 > winHeight - 1 || y1 < 0) {
        return;
    }

    if (x0 > x1) {
        int tmp = 0;

        tmp = x0;
        x0 = x1;
        x1 = tmp;

        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    dx = x1 - x0;
    dy = y1 - y0;

    // 两个坐标点相同则绘制点
    if (!dx && !dy) {
        drawPixel(SR_Vec2f(x0, y0), color);
        return;
    }

    i = x0;
    j = y0;

    /*
     * f(x, y) = f(x, y)
     * f(x + 1, y) = f(x, y) + (y0 - y1)
     * f(x, y + 1) = f(x, y) + (x1 - x0)
     * f(x + 1, y + 1) = f(x, y) + (y0 - y1) + (x1 - x0)
     * 
     * 有了以上公式，可以使绘制效率更高一点
     */

    if (dy > 0 && dy <= dx) {
        d = (float)((y0 - y1) * (x0 + 1)) + 
            ((float)(x1 - x0)) * ((float)(y0 + 0.5f)) +
            (float)(x0 * y1 - x1 * y0);

        // 斜率 (0, 1]，x 变化快于 y，以 x 为锚
        while (i <= x1) {
            drawPixel(SR_Vec2f(i, j), color);

            if (d < 0) {
                j++;
                d = d + (float)((x1 - x0) + (y0 - y1));
            } else {
                d = d + (float)(y0 - y1);
            }

            i++;
        }
    } else if (dy > 0 && dy > dx) {
        d = ((float)(y0 - y1)) * ((float)x0 + 0.5f) + 
            (float)(x1 - x0) * (y0 + 1) +
            (float)(x0 * y1 - x1 * y0);

        // 斜率 [1, infinity) y 的变化快于 x，以 y 为锚
        while (j <= y1) {
            drawPixel(SR_Vec2f(i, j), color);

            if (d > 0) {
                i++;
                d = d + (float)((x1 - x0) + (y0 - y1));
            } else {
                d = d + (float)(x1 - x0);
            }

            j++;
        }
    } else if (dy < 0 && -dy <= dx) {
        d = (float)((y0 - y1) * (x0 + 1)) + 
            ((float)(x1 - x0)) * ((float)(y0 - 0.5f)) +
            (float)(x0 * y1 - x1 * y0);

        // 斜率 [-1, 0)，x 变化快于 y，以 x 为锚
        while (i <= x1) {
            drawPixel(SR_Vec2f(i, j), color);

            if (d > 0) {
                j--;
                d = d - (float)(x1 - x0) + (float)(y0 - y1);
            } else {
                d = d + (float)(y0 - y1);
            }

            i++;
        }
    } else if (dy < 0 && -dy > dx) {
        // 斜率 (-infinite, -1)，y 变化快于 x，以 y 为锚
        d = ((float)(y0 - y1)) * ((float)x0 + 0.5f) + 
            (float)(x1 - x0) * (y0 - 1) +
            (float)(x0 * y1 - x1 * y0);

        // 斜率 [1, infinity) y 的变化快于 x，以 y 为锚
        while (j >= y1) {
            drawPixel(SR_Vec2f(i, j), color);

            if (d < 0) {
                i++;
                d = d - (float)(x1 - x0) + (float)(y0 - y1);
            } else {
                d = d - (float)(x1 - x0);
            }

            j--;
        }
    } else if (!dx) {
        // 绘制一条平行于 y 轴的竖线
        i = y0;
        j = y0 < y1 ? 1 : -1;

        while (i != y1) {
            drawPixel(SR_Vec2f(i, j), color);
            i += j;
        }
    } else if (!dy) {
        // 绘制一条平行于 x 轴的横线
        while (i <= x1) {
            drawPixel(SR_Vec2f(i, j), color);
            i++;
        }
    }
}

void SR_Window::render()
{
    SDL_Texture *texture = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event *evt = nullptr;

    if (!info) {
        return;
    }

    texture = info->texture;
    renderer = info->renderer;
    evt = &(info->evt);

    while (1) {
        while (SDL_PollEvent(evt)) {
            if (evt->type == SDL_QUIT) {
                return;
            }
        }

        int count = winWidth * winHeight;

        // 清屏
        Uint32 *buffer = info->surfaceBuffer;
        std::memset(buffer, 0, count * sizeof(Uint32));

        this->update(*this);

        SDL_UpdateTexture(texture, nullptr, info->surfaceBuffer, winWidth * sizeof(Uint32));
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }
}

int SR_Window::getWidth() const
{
    return winWidth;
}

int SR_Window::getHeight() const
{
    return winHeight;
}

//-----------------------------------------------------------------------------
