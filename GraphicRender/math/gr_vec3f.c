#include <math.h>
#include "gr_vec3f.h"

/*===========================================================================*/

GR_VEC3F gr_vec3f_add(const GR_VEC3F *a, const GR_VEC3F *b)
{
    GR_VEC3F c = { 0 };

    if (a && b) {
        c.x = a->x + b->x;
        c.y = a->y + b->y;
        c.z = a->z + b->z;
    }
    return c;
}

GR_VEC3F gr_vec3f_sub(const GR_VEC3F *a, const GR_VEC3F *b)
{
    GR_VEC3F c = { 0 };

    if (a && b) {
        c.x = a->x - b->x;
        c.y = a->y - b->y;
        c.z = a->z - b->z;
    }
    return c;
}

GR_VEC3F gr_vec3f_mul(const GR_VEC3F *a, float t)
{
    GR_VEC3F c = { 0 };

    if (a) {
        c.x = a->x * t;
        c.y = a->y * t;
        c.z = a->z * t;
    }
    return c;
}

GR_VEC3F gr_vec3f_div(const GR_VEC3F *a, float t)
{
    GR_VEC3F c = { 0 };

    if (a) {
        c.x = a->x / t;
        c.y = a->y / t;
        c.z = a->z / t;
    }
    return c;
}

GR_VEC3F gr_vec3f_reverse(const GR_VEC3F *a)
{
    GR_VEC3F c = { 0 };

    if (a) {
        c.x = -a->x;
        c.y = -a->y;
        c.z = -a->z;
    }
    return c;
}

GR_VEC3F gr_vec3f_normalize(const GR_VEC3F *a)
{
    GR_VEC3F c = { 0 };

    if (a) {
        float x = a->x;
        float y = a->y;
        float z = a->z;
        float len = sqrtf(x * x + y * y + z * z);

        if (fabsf(len) > 1e-6f) {
            c.x = x / len;
            c.y = y / len;
            c.z = z / len;
        }
    }
    return c;
}

GR_VEC3F gr_vec3f_lerp(const GR_VEC3F *a, const GR_VEC3F *b, float t)
{
    GR_VEC3F c = { 0 };
    GR_VEC3F d = { 0 };
    GR_VEC3F e = { 0 };

    if (t <= 0.0f) {
        t = 0.0f;
    } else if (t >= 1.0f) {
        t = 1.0f;
    }

    c = gr_vec3f_mul(a, 1.0f - t);
    d = gr_vec3f_mul(b, t);
    e = gr_vec3f_add(&c, &d);
    return e;
}

float gr_vec3f_dot(const GR_VEC3F *a, const GR_VEC3F *b)
{
    float c = 0.0f;

    if (a && b) {
        c = a->x * b->x + a->y * b->y + a->z * b->z;
    }
    return c;
}

GR_VEC3F gr_vec3f_cross(const GR_VEC3F *a, const GR_VEC3F *b)
{
    GR_VEC3F c = { 0 };

    /*
     * a × b = (xaX + yaY + zaZ) × (xbX + ybY + zbZ)
     *  = xaxbX × X + xaybX × Y + xazbX × Z + 
     *    yaxbY × X + yaybY × Y + yazbY × Z + 
     *    zaxbZ × X + zaybZ × Y + zazbZ × Z
     *  = (yazb − zayb)X + (zaxb − xazb)Y + (xayb − yaxb)Z.
     */
    if (a && b) {
        c.x = a->y * b->z - a->z * b->y;
        c.y = a->z * b->x - a->x * b->z;
        c.z = a->x * b->y - a->y * b->x;
    }
    return c;
}

float gr_vec3f_length(const GR_VEC3F *a)
{
    float len = 0.0f;

    if (a) {
        float x = a->x;
        float y = a->y;
        float z = a->z;
        len = sqrtf(x * x + y * y + z * z);
    }
    return len;
}

float gr_vec3f_angle(const GR_VEC3F *a, const GR_VEC3F *b)
{
    float c = 0.0f;

    if (a && b) {
        float ll = gr_vec3f_length(a) * gr_vec3f_length(b);
        if (fabsf(ll) > 0.0f) {
            float dot = gr_vec3f_dot(a, b);
            dot /= ll;

            c = acosf(dot);
        }
    }
    return c;
}

GR_VEC3F gr_vec3f_line_comb(const GR_VEC3F *vlist, const float *wlist, int k)
{
    GR_VEC3F c = { 0 };
    int i = 0;

    if (vlist && k > 0) {
        for (i = 0; i < k; i++) {
            c.x += (wlist[i] * vlist[i].x);
            c.y += (wlist[i] * vlist[i].y);
            c.y += (wlist[i] * vlist[i].y);
        }
    }

    return c;
}

/*===========================================================================*/
