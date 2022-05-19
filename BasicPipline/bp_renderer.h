#ifndef BP_RENDERER_H
#define BP_RENDERER_H

#include "bp_vector.h"

/*===========================================================================*/
/* 光栅化 */
/*===========================================================================*/

/* 指定屏幕大小（单位：像素） */
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360

/* 绘制类型 */
#define BP_DT_POINT 0
#define BP_DT_LINES (BP_DT_POINT + 1)
#define BP_DT_TRIANGLES (BP_DT_LINES + 1)

/* 像素颜色 */
struct bp_pixel_color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

/* 定义上下文 */
typedef struct bp_context BP_CONTEXT;

/* 初始化、终止上下文 */
BP_CONTEXT *bp_init(void (*before_render)(), void (*update)(), void (*after_render)());
void bp_render(BP_CONTEXT *ctx);
void bp_quit(BP_CONTEXT *ctx);

/* 在指定位置绘制指定颜色的像素 */
void bp_draw_pixel(int x, int y, const struct bp_pixel_color *color);

/* 中点算法绘制直线 */
void bp_draw_line(
    int x0, int y0,
    int x1, int y1,
    const struct bp_pixel_color *color
);

/* 填充三角形 */
void bp_fill_triangle(
    int x0, int y0,
    int x1, int y1,
    int x2, int y2,
    const struct bp_pixel_color *color
);

/* 填充三角形，使用中心坐标填充颜色 */
void bp_fill_triangle_ex(
    int x0, int y0,
    int x1, int y1,
    int x2, int y2,
    const struct bp_pixel_color *color0,
    const struct bp_pixel_color *color1,
    const struct bp_pixel_color *color2
);

/* 绘制顶点数组 */
void bp_draw_array(int draw_type, const struct vec4f_point *array, int count);

/*===========================================================================*/

#endif /* BP_RENDERER_H */
