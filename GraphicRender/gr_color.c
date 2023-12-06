#include "gr_color.h"
#include "gr_global.h"

/*===========================================================================*/

GR_COLOR gr_color_argb(float a, float r, float g, float b)
{
    GR_COLOR c;

    c.a = a;
    c.r = r;
    c.g = g;
    c.b = b;
    return c;
}

GR_COLOR gr_color_rgb(float r, float g, float b)
{
    GR_COLOR c;

    c.a = 1.0f;
    c.r = r;
    c.g = g;
    c.b = b;
    return c;
}

void gr_color_set_rgb(GR_COLOR *color, float r, float g, float b)
{
    if (color) {
        color->r = r;
        color->g = g;
        color->b = b;
    }
}

void gr_color_set_argb(GR_COLOR *color, float a, float r, float g, float b)
{
    if (color) {
        color->a = a;
        color->r = r;
        color->g = g;
        color->b = b;
    }
}

void gr_color_clamp(GR_COLOR *color)
{
    if (color) {
        color->r = gr_clampf(color->r, 1.0f, 0.0f);
        color->g = gr_clampf(color->g, 1.0f, 0.0f);
        color->b = gr_clampf(color->b, 1.0f, 0.0f);
        color->a = gr_clampf(color->a, 1.0f, 0.0f);
    }
}

GR_ARGB gr_color_to_argb(const GR_COLOR *color)
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

GR_COLOR gr_color_from_uint(GR_ARGB rgb)
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

GR_COLOR gr_color_add(const GR_COLOR *a, const GR_COLOR *b)
{
    GR_COLOR c = gr_color_argb(1.0f, 0.0f, 0.0f, 0.0f);

    if (a && b) {
        c.r = a->r + b->r;
        c.g = a->g + b->g;
        c.b = a->b + b->b;
    }
    return c;
}

GR_COLOR gr_color_sub(const GR_COLOR *a, const GR_COLOR *b)
{
    GR_COLOR c = gr_color_argb(1.0f, 0.0f, 0.0f, 0.0f);

    if (a && b) {
        c.r = a->r - b->r;
        c.g = a->g - b->g;
        c.b = a->b - b->b;
    }
    return c;
}

GR_COLOR gr_color_mul(const GR_COLOR *a, float t)
{
    GR_COLOR c = gr_color_argb(1.0f, 0.0f, 0.0f, 0.0f);

    if (a) {
        c.r = a->r * t;
        c.g = a->g * t;
        c.b = a->b * t;
    }
    return c;
}

GR_COLOR gr_color_div(const GR_COLOR *a, float t)
{
    GR_COLOR c = gr_color_argb(1.0f, 0.0f, 0.0f, 0.0f);

    if (a && t > 0.0f) {
        c.r = a->r / t;
        c.g = a->g / t;
        c.b = a->b / t;
    }
    return c;
}

GR_COLOR gr_color_lerp(const GR_COLOR *a, const GR_COLOR *b, float t)
{
    GR_COLOR c = gr_color_argb(1.0f, 0.0f, 0.0f, 0.0f);
    GR_COLOR d = gr_color_argb(1.0f, 0.0f, 0.0f, 0.0f);
    GR_COLOR e = gr_color_argb(1.0f, 0.0f, 0.0f, 0.0f);

    if (t <= 0.0f) {
        t = 0.0f;
    } else if (t >= 1.0f) {
        t = 1.0f;
    }

    c = gr_color_mul(a, 1.0f - t);
    d = gr_color_mul(b, t);
    e = gr_color_add(&c, &d);
    return e;
}

/*===========================================================================*/
