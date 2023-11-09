#ifndef __GR_COLOR_H__
#define __GR_COLOR_H__

#include "gr_type.h"

/*===========================================================================*/
/* 颜色 */
/*===========================================================================*/

/* 颜色对象 */
typedef struct gr_color
{
    float a;
    float r;
    float g;
    float b;
} GR_COLOR;

/* 颜色的整数表示从高到底分别是 A | R | G | B */
typedef GR_UINT32 GR_RGB;

/* 颜色值限制范围 */
void gr_color_clamp(GR_COLOR *color);

/* 将颜色对象转换为整数表示 */
GR_RGB gr_color_to_rgb(const GR_COLOR *color);

/* 将整数转换为颜色对象 */
GR_COLOR gr_color_to_obj(GR_RGB rgb);

/*===========================================================================*/

#endif /* __GR_COLOR_H__ */