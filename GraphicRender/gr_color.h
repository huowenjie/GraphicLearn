#ifndef __GR_COLOR_H__
#define __GR_COLOR_H__

#include "gr_type.h"

/*===========================================================================*/
/* 颜色 */
/*===========================================================================*/

/* 颜色对象 */
typedef struct
{
    float a;
    float r;
    float g;
    float b;
} GR_COLOR;

/* 颜色的整数表示从高到底分别是 A | R | G | B */
typedef GR_UINT32 GR_ARGB;

/* 构建颜色 */
GR_COLOR gr_color_argb(float a, float r, float g, float b);

/* 构建颜色 */
GR_COLOR gr_color_rgb(float r, float g, float b);

/* 设置颜色 */
void gr_color_set_rgb(GR_COLOR *color, float r, float g, float b);

/* 设置颜色 */
void gr_color_set_argb(GR_COLOR *color, float a, float r, float g, float b);

/* 颜色值限制范围 */
void gr_color_clamp(GR_COLOR *color);

/* 将颜色对象转换为整数表示 */
GR_ARGB gr_color_to_argb(const GR_COLOR *color);

/* 将整数转换为颜色对象 */
GR_COLOR gr_color_from_uint(GR_ARGB rgb);

/* 颜色加 */
GR_COLOR gr_color_add(const GR_COLOR *a, const GR_COLOR *b);

/* 颜色减 */
GR_COLOR gr_color_sub(const GR_COLOR *a, const GR_COLOR *b);

/* 颜色乘 */
GR_COLOR gr_color_mul(const GR_COLOR *a, float t);

/* 颜色除 */
GR_COLOR gr_color_div(const GR_COLOR *a, float t);

/* 颜色线性插值 p = (1 - t) * a + t * b 当 t = 0 时返回 a，否则返回 b */
GR_COLOR gr_color_lerp(const GR_COLOR *a, const GR_COLOR *b, float t);

/*===========================================================================*/

#endif /* __GR_COLOR_H__ */