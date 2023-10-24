#include "gr_color.h"
#include "gr_global.h"

/*===========================================================================*/

void gr_color_clamp(GR_COLOR *color)
{
    if (color) {
        color->r = gr_clampf(color->r, 1.0f, 0.0f);
        color->g = gr_clampf(color->r, 1.0f, 0.0f);
        color->b = gr_clampf(color->r, 1.0f, 0.0f);
        color->a = gr_clampf(color->r, 1.0f, 0.0f);
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
    return (ir << 16) | (ig << 8) | ib;
}

/*===========================================================================*/
