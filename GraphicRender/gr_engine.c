#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "gr_engine.h"
#include "gr_window.h"

#include "gr_mem.h"

/*===========================================================================*/

struct gr_engine
{
    /* 窗口对象 */
    GR_WINDOW *window;

    /* 帧缓冲的宽度和高度 */
    GR_UINT32 width;
    GR_UINT32 height;

    /* 颜色缓冲 */
    GR_UINT32 *cbuffer;

    /* 颜色子缓冲层，尺寸根据抗锯齿算法参数决定 */
    GR_UINT32 *child_cbuffer;

    /* 深度缓冲 */
    float *zbuffer;

    /* 深度子缓冲层，尺寸根据抗锯齿算法参数决定 */
    float *child_zbuffer;

    /* 子缓冲采样层次 1、0=不缩放 2=2x2 4=4x4 8=8x8 */
    GR_UINT32 sampler_layer;

    /* 当前抗锯齿方案 */
    ANTI_ALIASING current_aa;

    /* 更新回调 */
    GR_LOOP_UPDATE update;

    /* 回调参数 */
    void *args;
};

/*===========================================================================*/

/* 清空颜色缓冲区 */
static void clear_buffer(
    GR_UINT32 *buffer, float *zbuffer, GR_UINT32 size, GR_ARGB color,
    float depth);

/* 绘制像素 */
static void draw_pixel(GR_ENGINE *engine, int x, int y, const GR_COLOR *color);

/*===========================================================================*/

GR_ENGINE *gr_engine_create(int width, int height)
{
    GR_ENGINE *engine = NULL;
    GR_WINDOW *window = NULL;

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

    engine->window = window;
    engine->width = width;
    engine->height = height;
    engine->current_aa = NO_AA;

    return engine;

err:
    if (window) {
        gr_destroy_window(window);
    }

    if (engine) {
        GR_FREE(engine);
    }
    return NULL;
}

void gr_engine_create_buffer(GR_ENGINE *engine, GR_UINT32 layer)
{
    GR_UINT32 *cbuffer = NULL;
    GR_UINT32 *cchild = NULL;

    float *zbuffer = NULL;
    float *zchild = NULL;

    GR_UINT32 size = 0;
    GR_UINT32 sl = 0;

    if (!engine) {
        return;
    }

    switch (layer) {
        case 2:
        case 4:
        case 8:
            sl = layer;
            break;
        default:
            sl = 1;
            break;
    }

    size = engine->width * engine->height;

    cbuffer = GR_MALLOC(size * sizeof(GR_UINT32));
    zbuffer = GR_MALLOC(size * sizeof(float));

    if (!cbuffer || !zbuffer) {
        goto err;
    }

    memset(cbuffer, 0, size * sizeof(GR_UINT32));
    memset(zbuffer, 0, size * sizeof(float));
    clear_buffer(cbuffer, zbuffer, size, 0, -FLT_MAX);

    if (sl > 1) {
        size = size * sl * sl;

        cchild = GR_MALLOC(size * sizeof(GR_UINT32));
        zchild = GR_MALLOC(size * sizeof(float));

        if (!cchild || !zchild) {
            goto err;
        }

        memset(cchild, 0, size * sizeof(GR_UINT32));
        memset(zchild, 0, size * sizeof(float));
        clear_buffer(cchild, zchild, size, 0, -FLT_MAX);
    }

    engine->cbuffer = cbuffer;
    engine->zbuffer = zbuffer;
    engine->child_cbuffer = cchild;
    engine->child_zbuffer = zchild;
    engine->sampler_layer = layer;

    return;

err:
    if (cbuffer) {
        GR_FREE(cbuffer);
    }

    if (zbuffer) {
        GR_FREE(zbuffer);
    }

    if (cchild) {
        GR_FREE(cchild);
    }

    if (zchild) {
        GR_FREE(zchild);
    }
}

void gr_engine_destroy_buffer(GR_ENGINE *engine)
{
    if (engine) {
        if (engine->cbuffer) {
            GR_FREE(engine->cbuffer);
            engine->cbuffer = NULL;
        }

        if (engine->zbuffer) {
            GR_FREE(engine->zbuffer);
            engine->zbuffer = NULL;
        }

        if (engine->child_cbuffer) {
            GR_FREE(engine->child_cbuffer);
            engine->child_cbuffer = NULL;
        }

        if (engine->child_zbuffer) {
            GR_FREE(engine->child_zbuffer);
            engine->child_zbuffer = NULL;
        }

        engine->sampler_layer = 0;
    }
}

void gr_engine_destroy(GR_ENGINE *engine)
{
    if (engine) {
        if (engine->zbuffer) {
            GR_FREE(engine->zbuffer);
        }

        if (engine->cbuffer) {
            GR_FREE(engine->cbuffer);
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

int gr_engine_get_width(GR_ENGINE *engine)
{
    if (engine) {
        return engine->width;
    }
    return 0;
}

int gr_engine_get_height(GR_ENGINE *engine)
{
    if (engine) {
        return engine->height;
    }
    return 0;
}

void gr_engine_render(GR_ENGINE *engine)
{
    GR_WINDOW *window = NULL;
    GR_EVENT evt = { 0 };
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

    while (1) {
        while (gr_win_event_capture(window, &evt)) {
            if (evt.type == GR_EVT_WIN &&
                evt.winevt == GR_EVT_WIN_CLOSE) {
                return;
            }
        }

        int count = width * height;
        int sl = engine->sampler_layer;

        /* 清屏 */
        GR_UINT32 *buffer = engine->cbuffer;
        GR_UINT32 *child = engine->child_cbuffer;
        float *zbuffer = engine->zbuffer;
        float *zchild = engine->child_zbuffer;

        clear_buffer(buffer, zbuffer, count, 0, -FLT_MAX);
        clear_buffer(child, zchild, count * sl * sl, 0, -FLT_MAX);

        if (update) {
            update(engine, args);
        }

        gr_win_render(window, engine->cbuffer, width, height);
        gr_win_delay_ms(100);
    }
}

void gr_engine_set_aa(GR_ENGINE *engine, ANTI_ALIASING aa)
{
    if (engine) {
        engine->current_aa = aa;
    }
}

void gr_engine_draw_pixel(GR_ENGINE *engine, const GR_VEC2F *pt, const GR_COLOR *color)
{
    if (engine) {
        draw_pixel(engine, roundf(pt->x), roundf(pt->y), color);
    }
}

void gr_engine_draw_line(
    GR_ENGINE *engine, const GR_VEC2F *a, const GR_VEC2F *b, const GR_COLOR *color)
{
    int x0 = 0;
    int y0 = 0;
    int x1 = 0;
    int y1 = 0;

    int dx = 0;
    int dy = 0;

    int i = 0;
    int j = 0;

    int w = 0;
    int h = 0;
    float d = 0.0f;

    if (!engine || !a || !b || !color) {
        return;
    }

    x0 = roundf(a->x);
    y0 = roundf(a->y);
    x1 = roundf(b->x);
    y1 = roundf(b->y);

    i = x0;
    j = y0;

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

    /* 两个坐标点相同则绘制点 */
    if (!dx && !dy) {
        draw_pixel(engine, x0, y0, color);
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

        /* 斜率 (0, 1]，x 变化快于 y，以 x 为锚 */
        while (i <= x1) {
            draw_pixel(engine, i, j, color);

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

        /* 斜率 [1, infinity) y 的变化快于 x，以 y 为锚 */
        while (j <= y1) {
            draw_pixel(engine, i, j, color);

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

        /* 斜率 [-1, 0)，x 变化快于 y，以 x 为锚 */
        while (i <= x1) {
            draw_pixel(engine, i, j, color);

            if (d > 0) {
                j--;
                d = d - (float)(x1 - x0) + (float)(y0 - y1);
            } else {
                d = d + (float)(y0 - y1);
            }

            i++;
        }
    } else if (dy < 0 && -dy > dx) {
        /* 斜率 (-infinite, -1)，y 变化快于 x，以 y 为锚 */
        d = ((float)(y0 - y1)) * ((float)x0 + 0.5f) + 
            (float)(x1 - x0) * (y0 - 1) +
            (float)(x0 * y1 - x1 * y0);

        /* 斜率 [1, infinity) y 的变化快于 x，以 y 为锚 */
        while (j >= y1) {
            draw_pixel(engine, i, j, color);

            if (d < 0) {
                i++;
                d = d - (float)(x1 - x0) + (float)(y0 - y1);
            } else {
                d = d - (float)(x1 - x0);
            }

            j--;
        }
    } else if (!dx) {
        /* 绘制一条平行于 y 轴的竖线 */
        i = y0;
        j = y0 < y1 ? 1 : -1;

        while (i != y1) {
            draw_pixel(engine, i, j, color);
            i += j;
        }
    } else if (!dy) {
        /* 绘制一条平行于 x 轴的横线 */
        while (i <= x1) {
            draw_pixel(engine, i, j, color);
            i++;
        }
    }
}

/*===========================================================================*/

void clear_buffer(
    GR_UINT32 *buffer, float *zbuffer, GR_UINT32 size, GR_ARGB color,
    float depth)
{
    if (buffer && zbuffer) {
        int i = 0;
    
        for (; i < size; i++) {
            buffer[i] = color;
            zbuffer[i] = depth;
        }
    }
}

void draw_pixel(GR_ENGINE *engine, int x, int y, const GR_COLOR *color)
{
    int w = 0;
    int h = 0;

    GR_UINT32 *fbuffer = NULL;
    GR_ARGB rgb = 0;

    if (!engine || !color) {
        return;
    }

    rgb = gr_color_to_argb(color);
    fbuffer = engine->cbuffer;
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

/*===========================================================================*/
