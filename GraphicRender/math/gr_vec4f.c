#include <math.h>
#include "gr_vec4f.h"

/*===========================================================================*/

GR_VEC4F gr_vec4f_add(const GR_VEC4F *a, const GR_VEC4F *b)
{
    GR_VEC4F c = { 0 };

    if (a && b) {
        c.x = a->x + b->x;
        c.y = a->y + b->y;
        c.z = a->z + b->z;
        c.w = a->w + b->w;
    }
    return c;
}

GR_VEC4F gr_vec4f_sub(const GR_VEC4F *a, const GR_VEC4F *b)
{
    GR_VEC4F c = { 0 };

    if (a && b) {
        c.x = a->x - b->x;
        c.y = a->y - b->y;
        c.z = a->z - b->z;
        c.w = a->w - b->w;
    }
    return c;
}

GR_VEC4F gr_vec4f_mul(const GR_VEC4F *a, float t)
{
    GR_VEC4F c = { 0 };

    if (a) {
        c.x = a->x * t;
        c.y = a->y * t;
        c.z = a->z * t;
        c.w = a->w * t;
    }
    return c;
}

GR_VEC4F gr_vec4f_div(const GR_VEC4F *a, float t)
{
    GR_VEC4F c = { 0 };

    if (a) {
        c.x = a->x / t;
        c.y = a->y / t;
        c.z = a->z / t;
        c.w = a->w / t;
    }
    return c;
}

GR_VEC4F gr_vec4f_reverse(const GR_VEC4F *a)
{
    GR_VEC4F c = { 0 };

    if (a) {
        c.x = -a->x;
        c.y = -a->y;
        c.z = -a->z;
        c.w = -a->w;
    }
    return c;
}

GR_VEC4F gr_vec4f_lerp(const GR_VEC4F *a, const GR_VEC4F *b, float t)
{
    GR_VEC4F c = { 0 };
    GR_VEC4F d = { 0 };
    GR_VEC4F e = { 0 };

    if (t <= 0.0f) {
        t = 0.0f;
    } else if (t >= 1.0f) {
        t = 1.0f;
    }

    c = gr_vec4f_mul(a, 1.0f - t);
    d = gr_vec4f_mul(b, t);
    e = gr_vec4f_add(&c, &d);
    return e;
}

GR_VEC4F gr_vec4f_line_comb(const GR_VEC4F *vlist, const float *wlist, int k)
{
    GR_VEC4F c = { 0 };
    int i = 0;

    if (vlist && k > 0) {
        for (i = 0; i < k; i++) {
            c.x += (wlist[i] * vlist[i].x);
            c.y += (wlist[i] * vlist[i].y);
            c.z += (wlist[i] * vlist[i].z);
            c.w += (wlist[i] * vlist[i].w);
        }
    }

    return c;
}

GR_VEC4F gr_vec4f_homogen_div(const GR_VEC4F *a)
{
    GR_VEC4F c = { 0 };

    if (a && fabsf(a->w) > 1e-6f) {
        c.x = a->x / a->w;
        c.y = a->y / a->w;
        c.z = a->z / a->w;
        c.w = 1.0f;
    }
    return c;
}

/*===========================================================================*/
