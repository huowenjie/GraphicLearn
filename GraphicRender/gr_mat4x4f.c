#include <math.h>
#include "gr_mat3x3f.h"
#include "gr_mat4x4f.h"

/*===========================================================================*/

GR_MAT4X4F gr_mat4x4f_unit()
{
    GR_MAT4X4F mat;

    mat.m00 = 1.0f;
    mat.m01 = 0.0f;
    mat.m02 = 0.0f;
    mat.m03 = 0.0f;
    mat.m10 = 0.0f;
    mat.m11 = 1.0f;
    mat.m12 = 0.0f;
    mat.m13 = 0.0f;
    mat.m20 = 0.0f;
    mat.m21 = 0.0f;
    mat.m22 = 1.0f;
    mat.m23 = 0.0f;
    mat.m30 = 0.0f;
    mat.m31 = 0.0f;
    mat.m32 = 0.0f;
    mat.m33 = 1.0f;
    return mat;
}

GR_MAT4X4F gr_mat4x4f_zero()
{
    GR_MAT4X4F mat;

    mat.m00 = 0.0f;
    mat.m01 = 0.0f;
    mat.m02 = 0.0f;
    mat.m03 = 0.0f;
    mat.m10 = 0.0f;
    mat.m11 = 0.0f;
    mat.m12 = 0.0f;
    mat.m13 = 0.0f;
    mat.m20 = 0.0f;
    mat.m21 = 0.0f;
    mat.m22 = 0.0f;
    mat.m23 = 0.0f;
    mat.m30 = 0.0f;
    mat.m31 = 0.0f;
    mat.m32 = 0.0f;
    mat.m33 = 0.0f;
    return mat;
}

GR_MAT4X4F gr_mat4x4f_array(const float *elems)
{
    GR_MAT4X4F mat = gr_mat4x4f_zero();

    if (elems) {
        mat.m00 = elems[0];
        mat.m01 = elems[1];
        mat.m02 = elems[2];
        mat.m03 = elems[3];
        mat.m10 = elems[4];
        mat.m11 = elems[5];
        mat.m12 = elems[6];
        mat.m13 = elems[7];
        mat.m20 = elems[8];
        mat.m21 = elems[9];
        mat.m22 = elems[10];
        mat.m23 = elems[11];
        mat.m30 = elems[12];
        mat.m31 = elems[13];
        mat.m32 = elems[14];
        mat.m33 = elems[15];
    }
    return mat;
}

GR_MAT4X4F gr_mat4x4f_array2d(const float (*elems)[4])
{
    GR_MAT4X4F mat = gr_mat4x4f_zero();

    if (elems) {
        mat.m00 = elems[0][0];
        mat.m01 = elems[0][1];
        mat.m02 = elems[0][2];
        mat.m03 = elems[0][3];
        mat.m10 = elems[1][0];
        mat.m11 = elems[1][1];
        mat.m12 = elems[1][2];
        mat.m13 = elems[1][3];
        mat.m20 = elems[2][0];
        mat.m21 = elems[2][1];
        mat.m22 = elems[2][2];
        mat.m23 = elems[2][3];
        mat.m30 = elems[3][0];
        mat.m31 = elems[3][1];
        mat.m32 = elems[3][2];
        mat.m33 = elems[3][3];
    }
    return mat;
}

GR_MAT4X4F gr_mat4x4f_scale(float sx, float sy, float sz)
{
    GR_MAT4X4F mat = gr_mat4x4f_unit();

    mat.m00 = sx;
    mat.m11 = sx;
    mat.m22 = sx;

    return mat;
}

GR_MAT4X4F gr_mat4x4f_rotate_z(float theta)
{
    GR_MAT4X4F mat = gr_mat4x4f_unit();

    mat.m00 =  cosf(theta);
    mat.m01 = -sinf(theta);
    mat.m10 =  sinf(theta);
    mat.m11 =  cosf(theta);

    return mat;
}

GR_MAT4X4F gr_mat4x4f_rotate_x(float theta)
{
    GR_MAT4X4F mat = gr_mat4x4f_unit();

    mat.m11 =  cosf(theta);
    mat.m12 = -sinf(theta);
    mat.m21 =  sinf(theta);
    mat.m22 =  cosf(theta);

    return mat;
}

GR_MAT4X4F gr_mat4x4f_rotate_y(float theta)
{
    GR_MAT4X4F mat = gr_mat4x4f_unit();

    mat.m00 =  cosf(theta);
    mat.m20 = -sinf(theta);
    mat.m02 =  sinf(theta);
    mat.m22 =  cosf(theta);

    return mat;
}

GR_MAT4X4F gr_mat4x4f_translate(float dx, float dy, float dz)
{
    GR_MAT4X4F mat = gr_mat4x4f_unit();

    mat.m03 = dx;
    mat.m13 = dy;
    mat.m23 = dz;

    return mat;
}

GR_MAT4X4F gr_mat4x4f_volume_map(
    const GR_VEC3F *l,
    const GR_VEC3F *h,
    const GR_VEC3F *lp,
    const GR_VEC3F *hp
)
{
    GR_MAT4X4F mat = gr_mat4x4f_unit();

    if (l && h && lp && hp) {
        mat.m00 = (hp->x - lp->x) / (h->x - l->x);
        mat.m11 = (hp->y - lp->y) / (h->y - l->y);
        mat.m22 = (hp->z - lp->z) / (h->z - l->z);
        mat.m03 = (lp->x * h->x - hp->x * l->x) / (h->x - l->x);
        mat.m13 = (lp->y * h->y - hp->y * l->y) / (h->y - l->y);
        mat.m23 = (lp->z * h->z - hp->z * l->z) / (h->z - l->z);
    }

    return mat;
}

GR_MAT4X4F gr_mat4x4f_viewport(int width, int height)
{
    GR_MAT4X4F mat = gr_mat4x4f_unit();

    float nx = width;
    float ny = height;

    mat.m00 = nx / 2.0f;
    mat.m11 = ny / 2.0f;
    mat.m03 = (nx - 1.0f) / 2.0f;
    mat.m13 = (ny - 1.0f) / 2.0f;

    return mat;
}

GR_MAT4X4F gr_mat4x4f_camera(const GR_VEC3F *pos, const GR_VEC3F *target)
{
    GR_VEC3F z = { 0.0f, 1.0f, 0.0f };
    GR_VEC3F u = { 0 };
    GR_VEC3F v = { 0 };
    GR_VEC3F w = { 0 };

    GR_MAT4X4F cam = gr_mat4x4f_unit();

    if (!pos || !target) {
        return cam;
    }

    w = gr_vec3f_sub(pos, target);
    w = gr_vec3f_normalize(&w);

    u = gr_vec3f_cross(&z, &w);
    u = gr_vec3f_normalize(&u);

    v = gr_vec3f_cross(&w, &u);
    v = gr_vec3f_normalize(&v);

    cam.m00 = u.x;
    cam.m01 = u.y;
    cam.m02 = u.z;
    cam.m03 = -pos->x;

    cam.m10 = v.x;
    cam.m11 = v.y;
    cam.m12 = v.z;
    cam.m13 = -pos->y;

    cam.m20 = w.x;
    cam.m21 = w.y;
    cam.m22 = w.z;
    cam.m23 = -pos->z;

    return cam;
}

GR_MAT4X4F gr_mat4x4f_perspective(const GR_VEC3F *l, const GR_VEC3F *h)
{
    GR_MAT4X4F per = gr_mat4x4f_unit();

    if (l && h) {
        per.m33 = 0.0f;
        per.m32 = 1.0f;

        per.m00 = l->z * 2.0f / (h->x - l->x);
        per.m02 = (l->x + h->x) / (l->x - h->x);
        per.m11 = l->z * 2.0f / (h->y - l->y);
        per.m12 = (l->y + h->y) / (l->y - h->y);
        per.m22 = (l->z + h->z) / (l->z - h->z);
        per.m23 = 2.0 * l->z * h->z / (h->z - l->z);
    }
    return per;
}

GR_MAT4X4F gr_mat4x4f_transpose(const GR_MAT4X4F *mat)
{
    GR_MAT4X4F tmp = gr_mat4x4f_zero();

    if (mat) {
        tmp = *mat;
        tmp.m01 = mat->m10;
        tmp.m10 = mat->m01;
        tmp.m02 = mat->m20;
        tmp.m20 = mat->m02;
        tmp.m03 = mat->m30;
        tmp.m30 = mat->m03;
        tmp.m12 = mat->m21;
        tmp.m21 = mat->m12;
        tmp.m13 = mat->m31;
        tmp.m31 = mat->m13;
        tmp.m23 = mat->m32;
        tmp.m32 = mat->m23;
    }
    return tmp;
}

GR_MAT4X4F gr_mat4x4f_inverse(const GR_MAT4X4F *mat)
{
    GR_MAT4X4F tmp = gr_mat4x4f_zero();
    GR_MAT4X4F adj = gr_mat4x4f_zero();
    float det = 0.0f;

    if (!mat) {
        return tmp;
    }

    /* 求行列式 */
    det = gr_mat4x4f_determinant(mat);

    /* 行列式为 0，不可逆，直接返回原矩阵 */
    if (det == 0.0f || fabsf(det) < 1e-6f) {
        return *mat;
    }

    /*
     * 利用克拉默法则来求解逆矩阵
     * inverse A = adj A / det A
     * [ +c00, -c10, +c20, -c30 ]
     * [ -c01, +c11, -c21, +c31 ]
     * [ +c02, -c12, +c22, -c32 ]
     * [ -c03, +c13, -c23, +c33 ]
     */
    /* c00 */
    tmp.m00 = mat->m11; tmp.m01 = mat->m12; tmp.m02 = mat->m13;
    tmp.m10 = mat->m21; tmp.m11 = mat->m22; tmp.m12 = mat->m23;
    tmp.m20 = mat->m31; tmp.m21 = mat->m32; tmp.m22 = mat->m33;
    adj.m00 = gr_mat4x4f_determinant(&tmp) / det;

    /* c10 */
    tmp.m00 = mat->m01; tmp.m01 = mat->m02; tmp.m02 = mat->m03;
    tmp.m10 = mat->m21; tmp.m11 = mat->m22; tmp.m12 = mat->m23;
    tmp.m20 = mat->m31; tmp.m21 = mat->m32; tmp.m22 = mat->m33;
    adj.m01 = -(gr_mat4x4f_determinant(&tmp)) / det;

    /* c20 */
    tmp.m00 = mat->m01; tmp.m01 = mat->m02; tmp.m02 = mat->m03;
    tmp.m10 = mat->m11; tmp.m11 = mat->m12; tmp.m12 = mat->m13;
    tmp.m20 = mat->m31; tmp.m21 = mat->m32; tmp.m22 = mat->m33;
    adj.m02 = gr_mat4x4f_determinant(&tmp) / det;

    /* c30 */
    tmp.m00 = mat->m01; tmp.m01 = mat->m02; tmp.m02 = mat->m03;
    tmp.m10 = mat->m11; tmp.m11 = mat->m12; tmp.m12 = mat->m13;
    tmp.m20 = mat->m21; tmp.m21 = mat->m22; tmp.m22 = mat->m23;
    adj.m03 = -(gr_mat4x4f_determinant(&tmp)) / det;

    /* c01 */
    tmp.m00 = mat->m10; tmp.m01 = mat->m12; tmp.m02 = mat->m13;
    tmp.m10 = mat->m20; tmp.m11 = mat->m22; tmp.m12 = mat->m23;
    tmp.m20 = mat->m30; tmp.m21 = mat->m32; tmp.m22 = mat->m33;
    adj.m10 = -(gr_mat4x4f_determinant(&tmp)) / det;

    /* c11 */
    tmp.m00 = mat->m00; tmp.m01 = mat->m02; tmp.m02 = mat->m03;
    tmp.m10 = mat->m20; tmp.m11 = mat->m22; tmp.m12 = mat->m23;
    tmp.m20 = mat->m30; tmp.m21 = mat->m32; tmp.m22 = mat->m33;
    adj.m11 = gr_mat4x4f_determinant(&tmp) / det;

    /* c21 */
    tmp.m00 = mat->m00; tmp.m01 = mat->m02; tmp.m02 = mat->m03;
    tmp.m10 = mat->m10; tmp.m11 = mat->m12; tmp.m12 = mat->m13;
    tmp.m20 = mat->m30; tmp.m21 = mat->m32; tmp.m22 = mat->m33;
    adj.m12 = -(gr_mat4x4f_determinant(&tmp)) / det;

    /* c31 */
    tmp.m00 = mat->m00; tmp.m01 = mat->m02; tmp.m02 = mat->m03;
    tmp.m10 = mat->m10; tmp.m11 = mat->m12; tmp.m12 = mat->m13;
    tmp.m20 = mat->m20; tmp.m21 = mat->m22; tmp.m22 = mat->m23;
    adj.m13 = gr_mat4x4f_determinant(&tmp) / det;

    /* c02 */
    tmp.m00 = mat->m10; tmp.m01 = mat->m11; tmp.m02 = mat->m13;
    tmp.m10 = mat->m20; tmp.m11 = mat->m21; tmp.m12 = mat->m23;
    tmp.m20 = mat->m30; tmp.m21 = mat->m31; tmp.m22 = mat->m33;
    adj.m20 = gr_mat4x4f_determinant(&tmp) / det;

    /* c12 */
    tmp.m00 = mat->m00; tmp.m01 = mat->m01; tmp.m02 = mat->m03;
    tmp.m10 = mat->m20; tmp.m11 = mat->m21; tmp.m12 = mat->m23;
    tmp.m20 = mat->m30; tmp.m21 = mat->m31; tmp.m22 = mat->m33;
    adj.m21 = -(gr_mat4x4f_determinant(&tmp)) / det;

    /* c22 */
    tmp.m00 = mat->m00; tmp.m01 = mat->m01; tmp.m02 = mat->m03;
    tmp.m10 = mat->m10; tmp.m11 = mat->m11; tmp.m12 = mat->m13;
    tmp.m20 = mat->m30; tmp.m21 = mat->m31; tmp.m22 = mat->m33;
    adj.m22 = gr_mat4x4f_determinant(&tmp) / det;

    /* c32 */
    tmp.m00 = mat->m00; tmp.m01 = mat->m01; tmp.m02 = mat->m03;
    tmp.m10 = mat->m10; tmp.m11 = mat->m11; tmp.m12 = mat->m13;
    tmp.m20 = mat->m20; tmp.m21 = mat->m21; tmp.m22 = mat->m23;
    adj.m23 = -(gr_mat4x4f_determinant(&tmp)) / det;

    /* c03 */
    tmp.m00 = mat->m10; tmp.m01 = mat->m11; tmp.m02 = mat->m12;
    tmp.m10 = mat->m20; tmp.m11 = mat->m21; tmp.m12 = mat->m22;
    tmp.m20 = mat->m30; tmp.m21 = mat->m31; tmp.m22 = mat->m32;
    adj.m30 = -(gr_mat4x4f_determinant(&tmp)) / det;

    /* c13 */
    tmp.m00 = mat->m00; tmp.m01 = mat->m01; tmp.m02 = mat->m02;
    tmp.m10 = mat->m20; tmp.m11 = mat->m21; tmp.m12 = mat->m22;
    tmp.m20 = mat->m30; tmp.m21 = mat->m31; tmp.m22 = mat->m32;
    adj.m31 = gr_mat4x4f_determinant(&tmp) / det;

    /* c23 */
    tmp.m00 = mat->m00; tmp.m01 = mat->m01; tmp.m02 = mat->m02;
    tmp.m10 = mat->m10; tmp.m11 = mat->m11; tmp.m12 = mat->m12;
    tmp.m20 = mat->m30; tmp.m21 = mat->m31; tmp.m22 = mat->m32;
    adj.m32 = -(gr_mat4x4f_determinant(&tmp)) / det;

    /* c33 */
    tmp.m00 = mat->m00; tmp.m01 = mat->m01; tmp.m02 = mat->m02;
    tmp.m10 = mat->m10; tmp.m11 = mat->m11; tmp.m12 = mat->m12;
    tmp.m20 = mat->m20; tmp.m21 = mat->m21; tmp.m22 = mat->m22;
    adj.m33 = gr_mat4x4f_determinant(&tmp) / det;

    return adj;
}

float gr_mat4x4f_determinant(const GR_MAT4X4F *mat)
{
    float det = 0.0f;
    GR_MAT3X3F tmp = gr_mat3x3f_unit();

    if (mat) {
        tmp.m00 = mat->m11; tmp.m01 = mat->m12; tmp.m02 = mat->m13;
        tmp.m10 = mat->m21; tmp.m11 = mat->m22; tmp.m12 = mat->m23;
        tmp.m20 = mat->m31; tmp.m21 = mat->m32; tmp.m22 = mat->m33;

        det = mat->m00 * gr_mat3x3f_determinant(&tmp);

        tmp.m00 = mat->m10; tmp.m01 = mat->m12; tmp.m02 = mat->m13;
        tmp.m10 = mat->m20; tmp.m11 = mat->m22; tmp.m12 = mat->m23;
        tmp.m20 = mat->m30; tmp.m21 = mat->m32; tmp.m22 = mat->m33;

        det -= mat->m01 * gr_mat3x3f_determinant(&tmp);

        tmp.m00 = mat->m10; tmp.m01 = mat->m11; tmp.m02 = mat->m13;
        tmp.m10 = mat->m20; tmp.m11 = mat->m21; tmp.m12 = mat->m23;
        tmp.m20 = mat->m30; tmp.m21 = mat->m31; tmp.m22 = mat->m33;

        det += mat->m02 * gr_mat3x3f_determinant(&tmp);

        tmp.m00 = mat->m10; tmp.m01 = mat->m11; tmp.m02 = mat->m12;
        tmp.m10 = mat->m20; tmp.m11 = mat->m21; tmp.m12 = mat->m22;
        tmp.m20 = mat->m30; tmp.m21 = mat->m31; tmp.m22 = mat->m32;

        det -= mat->m03 * gr_mat3x3f_determinant(&tmp);
    }
    return det;
}

GR_MAT4X4F gr_mat4x4f_add(const GR_MAT4X4F *a, const GR_MAT4X4F *b)
{
    GR_MAT4X4F tmp = gr_mat4x4f_zero();

    if (a && b) {
        tmp.m00 = a->m00 + b->m00;
        tmp.m01 = a->m01 + b->m01;
        tmp.m02 = a->m02 + b->m02;
        tmp.m03 = a->m03 + b->m03;
        tmp.m10 = a->m10 + b->m10;
        tmp.m11 = a->m11 + b->m11;
        tmp.m12 = a->m12 + b->m12;
        tmp.m13 = a->m13 + b->m13;
        tmp.m20 = a->m20 + b->m20;
        tmp.m21 = a->m21 + b->m21;
        tmp.m22 = a->m22 + b->m22;
        tmp.m23 = a->m23 + b->m23;
        tmp.m30 = a->m30 + b->m30;
        tmp.m31 = a->m31 + b->m31;
        tmp.m32 = a->m32 + b->m32;
        tmp.m33 = a->m33 + b->m33;
    }
    return tmp;
}

GR_MAT4X4F gr_mat4x4f_sub(const GR_MAT4X4F *a, const GR_MAT4X4F *b)
{
    GR_MAT4X4F tmp = gr_mat4x4f_zero();

    if (a && b) {
        tmp.m00 = a->m00 - b->m00;
        tmp.m01 = a->m01 - b->m01;
        tmp.m02 = a->m02 - b->m02;
        tmp.m03 = a->m03 - b->m03;
        tmp.m10 = a->m10 - b->m10;
        tmp.m11 = a->m11 - b->m11;
        tmp.m12 = a->m12 - b->m12;
        tmp.m13 = a->m13 - b->m13;
        tmp.m20 = a->m20 - b->m20;
        tmp.m21 = a->m21 - b->m21;
        tmp.m22 = a->m22 - b->m22;
        tmp.m23 = a->m23 - b->m23;
        tmp.m30 = a->m30 - b->m30;
        tmp.m31 = a->m31 - b->m31;
        tmp.m32 = a->m32 - b->m32;
        tmp.m33 = a->m33 - b->m33;
    }
    return tmp;
}

GR_MAT4X4F gr_mat4x4f_mul(const GR_MAT4X4F *a, const GR_MAT4X4F *b)
{
    GR_MAT4X4F tmp = gr_mat4x4f_zero();

    if (a && b) {
        tmp.m00 = a->m00 * b->m00 + a->m01 * b->m10 + a->m02 * b->m20 + a->m03 * b->m30;
        tmp.m01 = a->m00 * b->m01 + a->m01 * b->m11 + a->m02 * b->m21 + a->m03 * b->m31;
        tmp.m02 = a->m00 * b->m02 + a->m01 * b->m12 + a->m02 * b->m22 + a->m03 * b->m32;
        tmp.m03 = a->m00 * b->m03 + a->m01 * b->m13 + a->m02 * b->m23 + a->m03 * b->m33;

        tmp.m10 = a->m10 * b->m00 + a->m11 * b->m10 + a->m12 * b->m20 + a->m13 * b->m30;
        tmp.m11 = a->m10 * b->m01 + a->m11 * b->m11 + a->m12 * b->m21 + a->m13 * b->m31;
        tmp.m12 = a->m10 * b->m02 + a->m11 * b->m12 + a->m12 * b->m22 + a->m13 * b->m32;
        tmp.m13 = a->m10 * b->m03 + a->m11 * b->m13 + a->m12 * b->m23 + a->m13 * b->m33;

        tmp.m20 = a->m20 * b->m00 + a->m21 * b->m10 + a->m22 * b->m20 + a->m23 * b->m30;
        tmp.m21 = a->m20 * b->m01 + a->m21 * b->m11 + a->m22 * b->m21 + a->m23 * b->m31;
        tmp.m22 = a->m20 * b->m02 + a->m21 * b->m12 + a->m22 * b->m22 + a->m23 * b->m32;
        tmp.m23 = a->m20 * b->m03 + a->m21 * b->m13 + a->m22 * b->m23 + a->m23 * b->m33;

        tmp.m30 = a->m30 * b->m00 + a->m31 * b->m10 + a->m32 * b->m20 + a->m33 * b->m30;
        tmp.m31 = a->m30 * b->m01 + a->m31 * b->m11 + a->m32 * b->m21 + a->m33 * b->m31;
        tmp.m32 = a->m30 * b->m02 + a->m31 * b->m12 + a->m32 * b->m22 + a->m33 * b->m32;
        tmp.m33 = a->m30 * b->m03 + a->m31 * b->m13 + a->m32 * b->m23 + a->m33 * b->m33;
    }
    return tmp;
}

GR_MAT4X4F gr_mat4x4f_mul_real(const GR_MAT4X4F *a, float b)
{
    GR_MAT4X4F tmp = gr_mat4x4f_zero();

    if (a) {
        tmp.m00 = a->m00 * b;
        tmp.m01 = a->m01 * b;
        tmp.m02 = a->m02 * b;
        tmp.m03 = a->m03 * b;
        tmp.m10 = a->m10 * b;
        tmp.m11 = a->m11 * b;
        tmp.m12 = a->m12 * b;
        tmp.m13 = a->m13 * b;
        tmp.m20 = a->m20 * b;
        tmp.m21 = a->m21 * b;
        tmp.m22 = a->m22 * b;
        tmp.m23 = a->m23 * b;
        tmp.m30 = a->m30 * b;
        tmp.m31 = a->m31 * b;
        tmp.m32 = a->m32 * b;
        tmp.m33 = a->m33 * b;
    }
    return tmp;
}

GR_VEC4F gr_mat4x4f_mul_vec(const GR_MAT4X4F *a, const GR_VEC4F *b)
{
    GR_VEC4F tmp = { 0 };

    if (a && b) {
        tmp.x = a->m00 * b->x + a->m01 * b->y + a->m02 * b->z + a->m03 * b->w;
        tmp.y = a->m10 * b->x + a->m11 * b->y + a->m12 * b->z + a->m13 * b->w;
        tmp.z = a->m20 * b->x + a->m21 * b->y + a->m22 * b->z + a->m23 * b->w;
        tmp.w = a->m30 * b->x + a->m31 * b->y + a->m32 * b->z + a->m33 * b->w;
    }
    return tmp;
}

/*===========================================================================*/
