#include <SDL2/SDL.h>
#include <cstring>
#include <cstdio>
#include "srt_window.h"

//-----------------------------------------------------------------------------

// 窗口信息
class SRT_WinCtx
{
public:
    SRT_WinCtx()
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

    void (*visitPixel)(const SRT_Vec2f &, const SRT_Vec2f &, SRT_Color &);

    // 帧缓冲
    Uint32 *surfaceBuffer;
};

SRT_Window::SRT_Window(int width, int height)
{
    info = new SRT_WinCtx();
    winWidth = width;
    winHeight = height;

    Uint32 *buffer = new Uint32[width * height];
    std::memset(buffer, 0, width * height * sizeof(Uint32));

    info->surfaceBuffer = buffer;
}

SRT_Window::~SRT_Window()
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

bool SRT_Window::initialize()
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
    info->visitPixel = nullptr;

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

bool SRT_Window::terminate()
{
    if (!info) {
        return false;
    }

    info->visitPixel = nullptr;

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

void SRT_Window::render()
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

        if (info->visitPixel) {
            SRT_Color color;
            SRT_Vec2f uv;
            SRT_Vec2f res((float)winWidth, (float)winHeight);

            // 逐像素访问
            for (int i = 0; i < count; i++) {
                color.setRGB(0.0f);

                int x = i % (winWidth);
                int y = i / (winWidth);

                // 将坐标归一化，将 y 的方向翻转，达到和 glsl shader 一样的效果
                uv.x = (float)(x) / ((float)winWidth - 1.0f);
                uv.y = 1.0f - (float)(y) / ((float)winHeight - 1.0f);

                // 将分辨率转换为浮点型也一并传入
                info->visitPixel(res, uv, color);
                buffer[i] = color.toUintRGB();
            }
        }

        SDL_UpdateTexture(texture, nullptr, info->surfaceBuffer, winWidth * sizeof(Uint32));
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }
}

void SRT_Window::setForeachPixelCallback(
    void (*visitPixel)(const SRT_Vec2f &, const SRT_Vec2f &, SRT_Color &)
)
{
    if (visitPixel) {
        info->visitPixel = visitPixel;
    }
}

//-----------------------------------------------------------------------------
