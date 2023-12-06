#include <stddef.h>
#include <string.h>

#include "gr_mem.h"
#include "gr_menu.h"
#include "gr_app.h"
#include "gr_global.h"
#include "gr_engine.h"
#include "gr_app.h"

/*===========================================================================*/

// 主菜单
static char basic_menu[][MENU_STR_LEN] =
{
    "基础渲染测试",
    "1) 点是否在三角形内",
    "2) 三角形交汇",
    "3) 片元着色器绘制 Piet Mondria",
    "0) 退出"
};

/*===========================================================================*/

/* 判断点是否在三角形内 */
static void point_in_triangle();
static GR_UINT32 pit_update(GR_ENGINE *engine, void *args);

/* 三个三角形交汇 */
static void three_triangle();
static GR_UINT32 three_triangle_update(GR_ENGINE *engine, void *args);

/* 绘制 Piet Mondria */
static void draw_pietmondria();
static GR_UINT32 draw_pietmondria_update(GR_ENGINE *engine, void *args);

/*===========================================================================*/

void render_basic()
{
    int index = 0;

    while (1) 
    {
        index = SELECT_MENU(basic_menu);
        switch (index) 
        {
        case 1:
            point_in_triangle();
            break;

        case 2:
            three_triangle();
            break;

        case 3:
            draw_pietmondria();
            break;

        case 0:
            return;
        }
    }
}

/*===========================================================================*/

void point_in_triangle()
{
    GR_ENGINE *engine = gr_engine_create(480, 360);

    gr_engine_create_buffer(engine, 2);
    gr_engine_set_update(engine, pit_update, NULL);

    gr_engine_render(engine);
    gr_engine_destroy_buffer(engine);
    gr_engine_destroy(engine);
}

GR_UINT32 pit_update(GR_ENGINE *engine, void *args)
{
    float width = (float)gr_engine_get_width(engine);
    float height = (float)gr_engine_get_height(engine);

    GR_VEC2F a = gr_vec2f_xy(width / 5.0f, height / 5.0f);
    GR_VEC2F b = gr_vec2f_xy(width * 4.0f / 5.0f, height / 5.0f);
    GR_VEC2F c = gr_vec2f_xy(width / 2.0f, height * 4.0f / 5.0f);
    GR_COLOR color = gr_color_rgb(0.0f, 1.0f, 0.0f);

    /* 待测样本 */
    int count = 256;
    int i = 0;

    GR_VEC2F *tp = GR_MALLOC(count * sizeof(GR_VEC2F));
    memset(tp, 0, count * sizeof(GR_VEC2F));

    gr_engine_draw_line(engine, &a, &b, &color);
    gr_engine_draw_line(engine, &b, &c, &color);
    gr_engine_draw_line(engine, &c, &a, &color);

    for (; i < count; i++) {
        int colorsel = 0;
        GR_VEC2F *pt = &tp[i];

        pt->x = randomf(1.0f, width - 2.0f);
        pt->y = randomf(1.0f, height - 2.0f);

        /* 随机五种颜色 */
        colorsel = randomi(1, 5);

        switch(colorsel) {
        case 1: gr_color_set_rgb(&color, 1.0f, 0.0f, 1.0f); break;
        case 2: gr_color_set_rgb(&color, 1.0f, 1.0f, 0.0f); break;
        case 3: gr_color_set_rgb(&color, 0.0f, 1.0f, 1.0f); break;
        case 4: gr_color_set_rgb(&color, 1.0f, 1.0f, 1.0f); break;
        case 5: gr_color_set_rgb(&color, 0.0f, 0.0f, 1.0f); break;
        }

        // 如果点在三角形内部，则颜色设置为和三角形一样的颜色
        if (gr_vec2f_inside_triangle(&a, &b, &c, pt)) {
            gr_color_set_rgb(&color, 0.0f, 1.0f, 0.0f);
        }

        gr_engine_draw_pixel(engine, pt, &color);
    }

    GR_FREE(tp);
    return 0;
}

/*===========================================================================*/

void three_triangle()
{
    GR_ENGINE *engine = gr_engine_create(480, 360);

    gr_engine_create_buffer(engine, 2);
    gr_engine_set_update(engine, three_triangle_update, NULL);

    gr_engine_render(engine);
    gr_engine_destroy_buffer(engine);
    gr_engine_destroy(engine);
}

GR_UINT32 three_triangle_update(GR_ENGINE *window, void *args)
{
    return 0;
}

/*===========================================================================*/

void draw_pietmondria()
{
    GR_ENGINE *engine = gr_engine_create(480, 360);

    gr_engine_create_buffer(engine, 2);
    gr_engine_set_update(engine, draw_pietmondria_update, NULL);

    gr_engine_render(engine);
    gr_engine_destroy_buffer(engine);
    gr_engine_destroy(engine);
}

GR_UINT32 draw_pietmondria_update(GR_ENGINE *window, void *args)
{
    return 0;
}

/*===========================================================================*/
