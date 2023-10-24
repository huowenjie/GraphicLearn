#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "gr_engine.h"
#include "gr_window.h"
#include "mem/gr_mem.h"

/*===========================================================================*/

struct gr_window
{
    SDL_Window *window;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    SDL_Event evt;
};

struct gr_engine
{
    GR_WINDOW *window;

    GR_UINT32 width;
    GR_UINT32 height;

    /* 帧缓冲 */
    GR_UINT32 *fbuffer;

    /* 深度缓冲 */
    float *zbuffer;

    /* 更新回调 */
    GR_LOOP_UPDATE update;

    /* 回调参数 */
    void *args;
};

GR_ENGINE *gr_engine_create(int width, int height)
{
    GR_ENGINE *engine = NULL;
    GR_WINDOW *window = NULL;
    GR_UINT32 *fbuffer = NULL;
    float *zbuffer = NULL;

    size_t fsize = 0;
    size_t zsize = 0;

    int i = 0;
    int count = width * height;

    if (width <= 0 || height <= 0) {
        return NULL;
    }

    engine = GR_MALLOC(sizeof(GR_ENGINE));
    if (!engine) {
        return NULL;
    }
    memset(engine, 0, sizeof(GR_ENGINE));

    window = gr_create_window(width, height);
    if (!window) {
        goto err;
    }

    fsize = sizeof(GR_UINT32) * count;
    zsize = sizeof(float) * count;

    fbuffer = GR_MALLOC(fsize);
    if (!fbuffer) {
        goto err;
    }
    memset(fbuffer, 0, fsize);

    zbuffer = GR_MALLOC(zsize);
    if (!zbuffer) {
        goto err;
    }
    memset(zbuffer, 0, zsize);
    
    for (i = 0; i < count; i++) {
        fbuffer[i] = 0x00000000;
        zbuffer[i] = -FLT_MAX;
    }

    engine->window = window;
    engine->width = width;
    engine->height = height;
    engine->fbuffer = fbuffer;
    engine->zbuffer = zbuffer;

    return engine;

err:
    if (zbuffer) {
        GR_FREE(zbuffer);
    }

    if (fbuffer) {
        GR_FREE(fbuffer);
    }

    if (window) {
        gr_destroy_window(window);
    }

    if (engine) {
        GR_FREE(engine);
    }
    return NULL;
}

void gr_engine_destroy(GR_ENGINE *engine)
{
    if (engine) {
        if (engine->zbuffer) {
            GR_FREE(engine->zbuffer);
        }

        if (engine->fbuffer) {
            GR_FREE(engine->fbuffer);
        }

        if (engine->window) {
            gr_destroy_window(engine->window);
        }

        if (engine) {
            GR_FREE(engine);
        }
    }
}

void gr_engine_set_update(GR_ENGINE *engine, GR_LOOP_UPDATE callback, void *args)
{
    if (engine) {
        engine->update = callback;
        engine->args = args;
    }
}

void gr_engine_render(GR_ENGINE *engine)
{
    GR_WINDOW *window = NULL;
    SDL_Texture *texture = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event *evt = NULL;
    GR_LOOP_UPDATE update = NULL;
    void *args = NULL;

    int width = 0;
    int height = 0;

    if (!engine) {
        return;
    }

    window = engine->window;
    if (!window) {
        return;
    }

    width = engine->width;
    height = engine->height;
    update = engine->update;
    args = engine->args;

    texture = window->texture;
    renderer = window->renderer;
    evt = &(window->evt);

    while (1) {
        while (SDL_PollEvent(evt)) {
            if (evt->type == SDL_QUIT) {
                return;
            }
        }

        int count = width * height;

        // 清屏
        GR_UINT32 *buffer = engine->fbuffer;
        float *zbuffer = engine->zbuffer;

        for (int i = 0; i < count; i++) {
            buffer[i] = 0x00000000;
            zbuffer[i] = -FLT_MAX;
        }

        if (update) {
            update(engine, args);
        }

        SDL_UpdateTexture(texture, NULL, engine->fbuffer, width * sizeof(Uint32));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }
}

void gr_engine_draw_pixel(GR_ENGINE *engine, int x, int y, GR_RGB rgb)
{    
    int w = 0;
    int h = 0;

    GR_UINT32 *fbuffer = NULL;

    if (!engine) {
        return;
    }

    fbuffer = engine->fbuffer;
    w = engine->width;
    h = engine->height;

    if (x > w - 1 || x < 0) {
        return;
    }

    if (y > h - 1 || y < 0) {
        return;
    }

    y = h - 1 - y;

    /* 点光栅化 */
    fbuffer[y * w + x] = rgb;
}

void gr_engine_draw_line(GR_ENGINE *engine, int ax, int ay, int bx, int by, GR_RGB rgb)
{
    int x0 = ax;
    int y0 = ay;
    int x1 = bx;
    int y1 = by;

    int dx = 0;
    int dy = 0;

    int i = x0;
    int j = y0;

    int w = 0;
    int h = 0;
    float d = 0.0f;

    if (!engine) {
        return;
    }

    w = engine->width;
    h = engine->height;

    if (x0 > w - 1 || x0 < 0 ||
        x1 > w - 1 || x1 < 0 ) {
        return;
    }

    if (y0 > h - 1 || y0 < 0 ||
        y1 > h - 1 || y1 < 0) {
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
        gr_engine_draw_pixel(engine, x0, y0, rgb);
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
            gr_engine_draw_pixel(engine, i, j, rgb);

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
            gr_engine_draw_pixel(engine, i, j, rgb);

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
            gr_engine_draw_pixel(engine, i, j, rgb);

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
            gr_engine_draw_pixel(engine, i, j, rgb);

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
            gr_engine_draw_pixel(engine, i, j, rgb);
            i += j;
        }
    } else if (!dy) {
        // 绘制一条平行于 x 轴的横线
        while (i <= x1) {
            gr_engine_draw_pixel(engine, i, j, rgb);
            i++;
        }
    }
}

/*---------------------------------------------------------------------------*/

GR_WINDOW *gr_create_window(int width, int height)
{
    GR_WINDOW *window = NULL;

    SDL_Window *sdlwnd = NULL;
    SDL_Texture *texture = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return NULL;
	}

    window = GR_MALLOC(sizeof(GR_WINDOW));
    if (!window) {
        goto err;
    }
    memset(window, 0, sizeof(GR_WINDOW));

    sdlwnd = SDL_CreateWindow(
        "GraphicRender",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        0
    );

    if (!sdlwnd) {
        goto err;
    }

    renderer = SDL_CreateRenderer(sdlwnd, -1, 0);
    if (!renderer) {
        goto err;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        width,
        height
    );

    if (!texture) {
        goto err;
    }

    window->window = sdlwnd;
    window->texture = texture;
    window->renderer = renderer;

    return window;

err:
    if (texture) {
        SDL_DestroyTexture(texture);
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }

    if (sdlwnd) {
        SDL_DestroyWindow(sdlwnd);
    }

    if (window) {
        GR_FREE(window);
    }

    SDL_Quit();
    return NULL;
}

void gr_destroy_window(GR_WINDOW *window)
{
    if (window) {
        if (window->texture) {
            SDL_DestroyTexture(window->texture);
        }

        if (window->renderer) {
            SDL_DestroyRenderer(window->renderer);
        }

        if (window->window) {
            SDL_DestroyWindow(window->window);
        }

        if (window) {
            GR_FREE(window);
        }

        SDL_Quit();
    }
}

/*===========================================================================*/
