#include <stddef.h>
#include <string.h>

#include "gr_mem.h"
#include "gr_menu.h"
#include "gr_app.h"
#include "gr_global.h"
#include "gr_engine.h"
#include "gr_app.h"

/*===========================================================================*/

/* 更新 */
static GR_UINT32 raytrace_update(GR_ENGINE *engine, void *args);

/* 绘制像素以以屏幕为中心的坐标系上 */
static void draw_view_pixel(GR_ENGINE *engine, float vw, float vh, const GR_VEC2F *pt, const GR_COLOR *color);

/*===========================================================================*/

void raytrace()
{
    GR_ENGINE *engine = gr_engine_create(480, 360);

    gr_engine_create_buffer(engine, 2);
    gr_engine_set_update(engine, raytrace_update, NULL);

    gr_engine_render(engine);
    gr_engine_destroy_buffer(engine);
    gr_engine_destroy(engine);
}

GR_UINT32 raytrace_update(GR_ENGINE *engine, void *args)
{
    GR_VEC2F pt = gr_vec2f_xy(30.0f, 30.0f);
    GR_COLOR color = gr_color_rgb(0.0f, 1.0f, 0.0f);

    float width = (float)gr_engine_get_width(engine);
    float height = (float)gr_engine_get_height(engine);

    pt.x = 0.0f;
    pt.y = 0.0f;
    draw_view_pixel(engine, width, height, &pt, &color);

    return 0;
}

void draw_view_pixel(GR_ENGINE *engine, float vw, float vh, const GR_VEC2F *pt, const GR_COLOR *color)
{
    GR_VEC2F tmp;
    tmp.x = pt->x + vw / 2.0f;
    tmp.y = pt->y + vh / 2.0f;
    gr_engine_draw_pixel(engine, &tmp, color);
}

/*===========================================================================*/
