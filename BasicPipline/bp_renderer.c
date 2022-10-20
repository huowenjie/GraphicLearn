#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <SDL2/SDL.h>

#include "bp_renderer.h"

/*===========================================================================*/

/* 帧缓冲*/
static Uint32 surfaceBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

/* z 缓冲 */
static float z_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

struct bp_context {
    SDL_Window *window;
    SDL_Texture *texture;
    SDL_Renderer *renderer;

    SDL_Event evt;

    void (*before_render)();
    void (*update)();
    void (*after_render)();
};

BP_CONTEXT *bp_init(void (*before_render)(), void (*update)(), void (*after_render)())
{
    BP_CONTEXT *ctx = NULL;
    SDL_Window *window = NULL;
    SDL_Texture *texture = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return NULL;
	}

    ctx = malloc(sizeof(*ctx));
    memset(ctx, 0, sizeof(*ctx));

    window = SDL_CreateWindow(
        "SDL2 Pixel Drawing",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
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
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );
    if (!texture) {
        goto err;
    }

    ctx->window = window;
    ctx->texture = texture;
    ctx->renderer = renderer;
    ctx->before_render = before_render;
    ctx->update = update;
    ctx->after_render = after_render;

    return ctx;

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

    free(ctx);
    SDL_Quit();
    return NULL;
}

void bp_render(BP_CONTEXT *ctx)
{
    SDL_Texture *texture = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event *evt = NULL;
    int i = 0;

    if (!ctx) {
        return;
    }

    texture = ctx->texture;
    renderer = ctx->renderer;
    evt = &(ctx->evt);

    if (ctx->before_render) {
        ctx->before_render();
    }

    while (1) {
        while (SDL_PollEvent(evt)) {
            if (evt->type == SDL_QUIT) {
                return;
            }
        }

/*
        SDL_WaitEvent(&evt);
        if (evt.type == SDL_QUIT) {
            goto end;
        }
*/
        for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
            surfaceBuffer[i] = 0x00000000;
            z_buffer[i] = -FLT_MAX;
        }

        if (ctx->update) {
            ctx->update();
        }

        SDL_UpdateTexture(texture, NULL, surfaceBuffer, SCREEN_WIDTH * sizeof(Uint32));
        /*SDL_RenderClear(renderer);*/
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }

    if (ctx->after_render) {
        ctx->after_render();
    }
}

void bp_quit(BP_CONTEXT *ctx)
{
    if (!ctx) {
        return;
    }

    if (ctx->texture) {
        SDL_DestroyTexture(ctx->texture);
    }

    if (ctx->renderer) {
        SDL_DestroyRenderer(ctx->renderer);
    }

    if (ctx->window) {
        SDL_DestroyWindow(ctx->window);
    }

    free(ctx);
    SDL_Quit();
}

void bp_draw_pixel(int x, int y, const struct bp_fragment *frag)
{
    if (x > SCREEN_WIDTH - 1 || x < 0) {
        return;
    }

    if (y > SCREEN_HEIGHT - 1 || y < 0) {
        return;
    }

    if (!frag) {
        return;
    }

    y = SCREEN_HEIGHT - 1 - y;

    /* 深度测试 */
    if (z_buffer[y * SCREEN_WIDTH + x] < frag->depth) {
        z_buffer[y * SCREEN_WIDTH + x] = frag->depth;

        /* 片段着色 */

        /* 点光栅化 */
        surfaceBuffer[y * SCREEN_WIDTH + x] = 
            0x00000000 | ((Uint32)(frag->r) << 16) | ((Uint32)(frag->g) <<  8) | frag->b;
    }
}

void bp_draw_line(
    int x0, int y0,
    int x1, int y1,
    const struct bp_fragment *frag
)
{
    int dx = 0;
    int dy = 0;

    int i = x0;
    int j = y0;

    float d = 0.0f;

    if (x0 > SCREEN_WIDTH - 1 || x0 < 0 ||
        x1 > SCREEN_WIDTH - 1 || x1 < 0 ) {
        return;
    }

    if (y0 > SCREEN_HEIGHT - 1 || y0 < 0 ||
        y1 > SCREEN_HEIGHT - 1 || y1 < 0) {
        return;
    }

    if (!frag) {
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
        bp_draw_pixel(x0, y0, frag);
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
            bp_draw_pixel(i, j, frag);

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
            bp_draw_pixel(i, j, frag);

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
            bp_draw_pixel(i, j, frag);

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
            bp_draw_pixel(i, j, frag);

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
            bp_draw_pixel(i, j, frag);

            i += j;
        }
    } else if (!dy) {
        /* 绘制一条平行于 x 轴的横线 */
        while (i <= x1) {
            bp_draw_pixel(i, j, frag);

            i++;
        }
    }
}

void bp_fill_triangle(
    int x0, int y0,
    int x1, int y1,
    int x2, int y2,
    const struct bp_fragment *frag0,
    const struct bp_fragment *frag1,
    const struct bp_fragment *frag2
)
{
    int xmin = x0;
    int xmax = x0;
    int ymin = y0;
    int ymax = y0;

    int i = 0;
    int j = 0;

    float fa = 0.0f;
    float fb = 0.0f;
    float fc = 0.0f;

    if (x0 > SCREEN_WIDTH - 1 || x0 < 0 ||
        x1 > SCREEN_WIDTH - 1 || x1 < 0 ||
        x2 > SCREEN_WIDTH - 1 || x2 < 0) {
        return;
    }

    if (y0 > SCREEN_HEIGHT - 1 || y0 < 0 ||
        y1 > SCREEN_HEIGHT - 1 || y1 < 0 ||
        y2 > SCREEN_HEIGHT - 1 || y2 < 0) {
        return;
    }

    if (!frag0 || !frag1 || !frag2) {
        return;
    }

    /*
     * 1.确定三角形的包围盒 xmin xmax ymin ymax；
     * 2.将像素点索引代入两点式求出重心坐标系数 a，b，c；
     * 3.拟定一个屏幕外的点 (-1, -1)，两个三角形共线时，以共线为分隔线，其中一个三角形的
     *   边上的像素点必定和该屏幕外点处于同一侧，以此来判断是否绘制；
     * 4.根据 a、b、c 来确定像素点的颜色
     * 
     * f01(x, y) = (y0 - y1)x + (x1 - x0)y + x0y1 - x1y0
     * f12(x, y) = (y1 - y2)x + (x2 - x1)y + x1y2 - x2y1
     * f20(x, y) = (y2 - y0)x + (x0 - x2)y + x2y0 - x0y2
     * 
     * a = f12(x, y) / f12(x0, y0)
     * b = f20(x, y) / f20(x1, y1)
     * c = f01(x, y) / f01(x2, y2)
     */
    xmin = xmin < x1 ? xmin : x1;
    xmin = xmin < x2 ? xmin : x2;
    xmax = xmax > x1 ? xmax : x1;
    xmax = xmax > x2 ? xmax : x2;

    ymin = ymin < y1 ? ymin : y1;
    ymin = ymin < y2 ? ymin : y2;
    ymax = ymax > y1 ? ymax : y1;
    ymax = ymax > y2 ? ymax : y2;

    fa = (y1 - y2) * x0 + (x2 - x1) * y0 + x1 * y2 - x2 * y1;
    fb = (y2 - y0) * x1 + (x0 - x2) * y1 + x2 * y0 - x0 * y2;
    fc = (y0 - y1) * x2 + (x1 - x0) * y2 + x0 * y1 - x1 * y0;

    for (i = ymin; i <= ymax; i++) {
        for (j = xmin; j <= xmax; j++) {
            float a = ((y1 - y2) * j + (x2 - x1) * i + x1 * y2 - x2 * y1) / fa;
            float b = ((y2 - y0) * j + (x0 - x2) * i + x2 * y0 - x0 * y2) / fb;
            float c = ((y0 - y1) * j + (x1 - x0) * i + x0 * y1 - x1 * y0) / fc;

            if (a >= 0.0f && b >= 0.0f && c >= 0.0f) {
                float ta = fa * ((y1 - y2) * (-1) + (x2 - x1) * (-1) + x1 * y2 - x2 * y1);
                float tb = fb * ((y2 - y0) * (-1) + (x0 - x2) * (-1) + x2 * y0 - x0 * y2);
                float tc = fc * ((y0 - y1) * (-1) + (x1 - x0) * (-1) + x0 * y1 - x1 * y0);

                if ((a >= 0.0f || ta > 0.0f) &&
                    (b >= 0.0f || tb > 0.0f) &&
                    (c >= 0.0f || tc > 0.0f)) {
                    Uint32 red = a * frag0->r + b * frag1->r + c * frag2->r;
                    Uint32 green = a * frag0->g + b * frag1->g + c * frag2->g;
                    Uint32 blue = a * frag0->b + b * frag1->b + c * frag2->b;

                    /* 利用重心坐标计算深度信息 */
                    float depth = a * frag0->depth + b * frag1->depth + c * frag2->depth;

                    struct bp_fragment frag = {
                        red, green, blue, depth 
                    };

                    bp_draw_pixel(j, i, &frag);
                }
            }
        }
    }
}

/* 绘制顶点 */
static void bp_draw_points(const struct vec4f_point *array, int count)
{
    int i = 0;
    int x = 0;
    int y = 0;
    struct bp_fragment frag = { 0 };

    while (i < count) {
        x = (int)array[i].x;
        y = (int)array[i].y;

        frag.r = (unsigned char)((SDL_clamp(array[i].r, 0.0f, 1.0f)) * 255.0f);
        frag.g = (unsigned char)((SDL_clamp(array[i].g, 0.0f, 1.0f)) * 255.0f);
        frag.b = (unsigned char)((SDL_clamp(array[i].g, 0.0f, 1.0f)) * 255.0f);
        frag.depth = -array[i].z;

        bp_draw_pixel(x, y, &frag);
        i++;
    }
}

/* 绘制线段，两两一组 */
static void bp_draw_lines(const struct vec4f_point *array, int count)
{
    int i = count - 1;
    struct bp_fragment frag = { 0 };

    if ((count == 1) || (count % 2)) {
        frag.r = (unsigned char)((SDL_clamp(array[i].r, 0.0f, 1.0f)) * 255.0f);
        frag.g = (unsigned char)((SDL_clamp(array[i].g, 0.0f, 1.0f)) * 255.0f);
        frag.b = (unsigned char)((SDL_clamp(array[i].g, 0.0f, 1.0f)) * 255.0f);
        frag.depth = -array[i].z;

        bp_draw_pixel(array[i].x, array[i].y, &frag);
        count--;
    }

    for (i = 0; i < count; i += 2) {
        frag.r = (unsigned char)((SDL_clamp(array[i].r, 0.0f, 1.0f)) * 255.0f);
        frag.g = (unsigned char)((SDL_clamp(array[i].g, 0.0f, 1.0f)) * 255.0f);
        frag.b = (unsigned char)((SDL_clamp(array[i].b, 0.0f, 1.0f)) * 255.0f);
        frag.depth = -array[i].z;

        bp_draw_line(
            array[i].x, array[i].y,
            array[i + 1].x, array[i + 1].y,
            &frag
        );
    }
}

/* 绘制三角形，三三一组 */
static void bp_draw_triangles(const struct vec4f_point *array, int count)
{
    int i = 0;
    struct bp_fragment fraga = { 0 };
    struct bp_fragment fragb = { 0 };
    struct bp_fragment fragc = { 0 };

    if (count < 3) {
        return;
    }

    count = count - (count % 3);
    for (; i < count; i += 3) {
        fraga.r = (unsigned char)((SDL_clamp(array[i].r, 0.0f, 1.0f)) * 255.0f);
        fraga.g = (unsigned char)((SDL_clamp(array[i].g, 0.0f, 1.0f)) * 255.0f);
        fraga.b = (unsigned char)((SDL_clamp(array[i].b, 0.0f, 1.0f)) * 255.0f);
        fraga.depth = -array[i].z;

        fragb.r = (unsigned char)((SDL_clamp(array[i + 1].r, 0.0f, 1.0f)) * 255.0f);
        fragb.g = (unsigned char)((SDL_clamp(array[i + 1].g, 0.0f, 1.0f)) * 255.0f);
        fragb.b = (unsigned char)((SDL_clamp(array[i + 1].b, 0.0f, 1.0f)) * 255.0f);
        fragb.depth = -array[i + 1].z;

        fragc.r = (unsigned char)((SDL_clamp(array[i + 2].r, 0.0f, 1.0f)) * 255.0f);
        fragc.g = (unsigned char)((SDL_clamp(array[i + 2].g, 0.0f, 1.0f)) * 255.0f);
        fragc.b = (unsigned char)((SDL_clamp(array[i + 2].b, 0.0f, 1.0f)) * 255.0f);
        fragc.depth = -array[i + 2].z;

        bp_fill_triangle(
            array[i].x, array[i].y,
            array[i + 1].x, array[i + 1].y,
            array[i + 2].x, array[i + 2].y,
            &fraga, &fragb, &fragc
        );
    }
}

void bp_draw_array(int draw_type, const struct vec4f_point *array, int count)
{
    if (!array || count <= 0) {
        return;
    }

    switch (draw_type) {
    case BP_DT_POINT:
        bp_draw_points(array, count);
        break;

    case BP_DT_LINES:
        bp_draw_lines(array, count);
        break;

    case BP_DT_TRIANGLES:
        bp_draw_triangles(array, count);
        break;

    default:
        break;
    }
}

/*===========================================================================*/
