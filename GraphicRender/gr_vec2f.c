#include <math.h>
#include "gr_vec2f.h"

/*===========================================================================*/

GR_VEC2F gr_vec2f_xy(float x, float y)
{
    GR_VEC2F v;
    
    v.x = x;
    v.y = y;
    return v;
}

GR_VEC2F gr_vec2f_add(const GR_VEC2F *a, const GR_VEC2F *b)
{
    GR_VEC2F c = { 0 };

    if (a && b) {
        c.x = a->x + b->x;
        c.y = a->y + b->y;
    }
    return c;
}

GR_VEC2F gr_vec2f_sub(const GR_VEC2F *a, const GR_VEC2F *b)
{
    GR_VEC2F c = { 0 };

    if (a && b) {
        c.x = a->x - b->x;
        c.y = a->y - b->y;
    }
    return c;
}

GR_VEC2F gr_vec2f_mul(const GR_VEC2F *a, float t)
{
    GR_VEC2F c = { 0 };

    if (a) {
        c.x = a->x * t;
        c.y = a->y * t;
    }
    return c;
}

GR_VEC2F gr_vec2f_div(const GR_VEC2F *a, float t)
{
    GR_VEC2F c = { 0 };

    if (a) {
        c.x = a->x / t;
        c.y = a->y / t;
    }
    return c;
}

GR_VEC2F gr_vec2f_reverse(const GR_VEC2F *a)
{
    GR_VEC2F c = { 0 };

    if (a) {
        c.x = -a->x;
        c.y = -a->y;
    }
    return c;
}

GR_VEC2F gr_vec2f_normalize(const GR_VEC2F *a)
{
    GR_VEC2F c = { 0 };

    if (a) {
        float x = a->x;
        float y = a->y;
        float len = sqrtf(x * x + y * y);

        if (fabsf(len) > 1e-6f) {
            c.x = x / len;
            c.y = y / len;
        }
    }
    return c;
}

GR_VEC2F gr_vec2f_lerp(const GR_VEC2F *a, const GR_VEC2F *b, float t)
{
    GR_VEC2F c = { 0 };
    GR_VEC2F d = { 0 };
    GR_VEC2F e = { 0 };

    if (t <= 0.0f) {
        t = 0.0f;
    } else if (t >= 1.0f) {
        t = 1.0f;
    }

    c = gr_vec2f_mul(a, 1.0f - t);
    d = gr_vec2f_mul(b, t);
    e = gr_vec2f_add(&c, &d);
    return e;
}

float gr_vec2f_dot(const GR_VEC2F *a, const GR_VEC2F *b)
{
    float c = 0.0f;

    if (a && b) {
        c = a->x * b->x + a->y * b->y;
    }
    return c;
}

float gr_vec2f_cross(const GR_VEC2F *a, const GR_VEC2F *b)
{
    float c = 0.0f;

    if (a && b) {
        c = a->x * b->y - a->y * b->x;
    }
    return c;
}

float gr_vec2f_length(const GR_VEC2F *a)
{
    float len = 0.0f;

    if (a) {
        float x = a->x;
        float y = a->y;
        len = sqrtf(x * x + y * y);
    }
    return len;
}

float gr_vec2f_angle(const GR_VEC2F *a, const GR_VEC2F *b)
{
    float c = 0.0f;

    if (a && b) {
        float ll = gr_vec2f_length(a) * gr_vec2f_length(b);
        if (fabsf(ll) > 0.0f) {
            float dot = gr_vec2f_dot(a, b);
            dot /= ll;

            c = acosf(dot);
        }
    }
    return c;
}

GR_VEC2F gr_vec2f_line_comb(const GR_VEC2F *vlist, const float *wlist, int k)
{
    GR_VEC2F c = { 0 };
    int i = 0;

    if (vlist && k > 0) {
        for (i = 0; i < k; i++) {
            c.x += (wlist[i] * vlist[i].x);
            c.y += (wlist[i] * vlist[i].y);
        }
    }

    return c;
}

GR_BOOL gr_vec2f_inside_triangle(
    const GR_VEC2F *a,
    const GR_VEC2F *b,
    const GR_VEC2F *c,
    const GR_VEC2F *p)
{
    /* 采用叉乘法判断，也可以采用重心法判断 */
    GR_VEC2F pa = gr_vec2f_sub(p, a);
    GR_VEC2F pb = gr_vec2f_sub(p, b);
    GR_VEC2F pc = gr_vec2f_sub(p, c);

    float z1 = gr_vec2f_cross(&pa, &pb);
    float z2 = gr_vec2f_cross(&pb, &pc);
    float z3 = gr_vec2f_cross(&pc, &pa);

    return z1 * z2 > 0 && z2 * z3 > 0;
}

/*===========================================================================*/
