#include <SDL2/SDL.h>
#include <cstring>
#include "srt_window.h"

//-----------------------------------------------------------------------------

// 窗口信息
class SRT_WinCtx
{
public:
    SRT_WinCtx() {
        window = NULL;
        texture = NULL;
        renderer = NULL;

        start = NULL;
        update = NULL;
        end = NULL;

        surfaceBuffer = NULL;
    }
public:
    SDL_Window *window;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    SDL_Event evt;

    void (*start)();
    void (*update)();
    void (*end)();

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
            info->surfaceBuffer = NULL;
        }
        delete info;
        info = NULL;
    }
}

bool SRT_Window::initialize()
{    
    SDL_Window *window = NULL;
    SDL_Texture *texture = NULL;
    SDL_Renderer *renderer = NULL;

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
    info->start = NULL;
    info->update = NULL;
    info->end = NULL;

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

    info->start = NULL;
    info->update = NULL;
    info->end = NULL;

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
    SDL_Texture *texture = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event *evt = NULL;

    if (!info) {
        return;
    }

    texture = info->texture;
    renderer = info->renderer;
    evt = &(info->evt);

    if (info->start) {
        info->start();
    }

    while (1) {
        while (SDL_PollEvent(evt)) {
            if (evt->type == SDL_QUIT) {
                return;
            }
        }

        // 清屏
        Uint32 *buffer = info->surfaceBuffer;
        std::memset(buffer, 0, winWidth * winHeight * sizeof(Uint32));

        if (info->update) {
            info->update();
        }

        SDL_UpdateTexture(texture, NULL, info->surfaceBuffer, winWidth * sizeof(Uint32));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }

    if (info->end) {
        info->end();
    }
}

void SRT_Window::setStartCallback(void (*start)())
{
    if (info) {
        info->start = start;
    }
}

void SRT_Window::setUpdateCallback(void (*update)())
{
    if (info) {
        info->update = update;
    }
}

void SRT_Window::setEndCallback(void (*end)())
{
    if (info) {
        info->end = end;
    }
}

//-----------------------------------------------------------------------------
