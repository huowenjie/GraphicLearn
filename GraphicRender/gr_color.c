#include "gr_color.h"
#include "gr_global.h"

/*===========================================================================*/

void gr_color_clamp(GR_COLOR *color)
{
    if (color) {
        color->r = gr_clampf(color->r, 1.0f, 0.0f);
        color->g = gr_clampf(color->g, 1.0f, 0.0f);
        color->b = gr_clampf(color->b, 1.0f, 0.0f);
        color->a = gr_clampf(color->a, 1.0f, 0.0f);
    }
}

GR_RGB gr_color_to_rgb(const GR_COLOR *color)
{
    if (!color) {
        return 0;
    }

    GR_UINT32 ir = (GR_UINT32)(color->r * 255.0f);
    GR_UINT32 ig = (GR_UINT32)(color->g * 255.0f);
    GR_UINT32 ib = (GR_UINT32)(color->b * 255.0f);
    GR_UINT32 ia = (GR_UINT32)(color->a * 255.0f);
    return (ia << 24) | (ir << 16) | (ig << 8) | ib;
}

GR_COLOR gr_color_to_obj(GR_RGB rgb)
{
    GR_COLOR color = { 0 };

    float a = (float)((rgb & 0xFF000000) >> 24);
    float r = (float)((rgb & 0x00FF0000) >> 16);
    float g = (float)((rgb & 0x0000FF00) >> 8);
    float b = (float)(rgb & 0x000000FF);

    color.a = a / 255.0f;
    color.r = r / 255.0f;
    color.g = g / 255.0f;
    color.b = b / 255.0f;
    return color;
}

/*===========================================================================*/
