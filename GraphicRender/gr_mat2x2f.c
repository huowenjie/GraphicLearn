#include <math.h>
#include "gr_mat2x2f.h"

/*===========================================================================*/

GR_MAT2X2F gr_mat2x2f_unit()
{
    GR_MAT2X2F mat;
    
    mat.m00 = 1.0f;
    mat.m01 = 0.0f;
    mat.m10 = 0.0f;
    mat.m11 = 1.0f;
    return mat;
}

GR_MAT2X2F gr_mat2x2f_zero()
{
    GR_MAT2X2F mat;

    mat.m00 = 0.0f;
    mat.m01 = 0.0f;
    mat.m10 = 0.0f;
    mat.m11 = 0.0f;
    return mat;
}

GR_MAT2X2F gr_mat2x2f_array(const float *elems)
{
    GR_MAT2X2F mat = gr_mat2x2f_zero();

    if (elems) {
        mat.m00 = elems[0];
        mat.m01 = elems[1];
        mat.m10 = elems[2];
        mat.m11 = elems[3];
    }
    return mat;
}

GR_MAT2X2F gr_mat2x2f_array2d(const float (*elems)[2])
{
    GR_MAT2X2F mat = gr_mat2x2f_zero();

    if (elems) {
        mat.m00 = elems[0][0];
        mat.m01 = elems[0][1];
        mat.m10 = elems[1][0];
        mat.m11 = elems[1][1];
    }
    return mat;
}

GR_MAT2X2F gr_mat2x2f_transpose(const GR_MAT2X2F *mat)
{
    GR_MAT2X2F tmp = gr_mat2x2f_zero();

    if (mat) {
        tmp = *mat;
        tmp.m01 = mat->m10;
        tmp.m10 = mat->m01;
    }

    return tmp;
}

GR_MAT2X2F gr_mat2x2f_inverse(const GR_MAT2X2F *mat)
{
    GR_MAT2X2F tmp = gr_mat2x2f_zero();
    float det = 0.0f;

    if (!mat) {
        return tmp;
    }

    /* 求行列式 */
    det = gr_mat2x2f_determinant(mat);

    /* 行列式为 0，不可逆，直接返回原矩阵 */
    if (det == 0.0f || fabsf(det) < 1e-6f) {
        return *mat;
    }

    tmp.m00 =  mat->m11;
    tmp.m01 = -mat->m01;
    tmp.m10 = -mat->m10;
    tmp.m11 =  mat->m00;

    det = 1.0f / det;
    tmp = gr_mat2x2f_mul_real(&tmp, det);
    return tmp;
}

float gr_mat2x2f_determinant(const GR_MAT2X2F *mat)
{
    float det = 0.0f;

    if (mat) {
        det = mat->m00 * mat->m11 - mat->m01 * mat->m10;
    }
    return det;
}

GR_MAT2X2F gr_mat2x2f_add(const GR_MAT2X2F *a, const GR_MAT2X2F *b)
{
    GR_MAT2X2F tmp = gr_mat2x2f_zero();

    if (a && b) {
        tmp.m00 = a->m00 + b->m00;
        tmp.m01 = a->m01 + b->m01;
        tmp.m10 = a->m10 + b->m10;
        tmp.m11 = a->m11 + b->m11;
    }

    return tmp;
}

GR_MAT2X2F gr_mat2x2f_sub(const GR_MAT2X2F *a, const GR_MAT2X2F *b)
{
    GR_MAT2X2F tmp = gr_mat2x2f_zero();

    if (a && b) {
        tmp.m00 = a->m00 - b->m00;
        tmp.m01 = a->m01 - b->m01;
        tmp.m10 = a->m10 - b->m10;
        tmp.m11 = a->m11 - b->m11;
    }

    return tmp;
}

GR_MAT2X2F gr_mat2x2f_mul(const GR_MAT2X2F *a, const GR_MAT2X2F *b)
{
    GR_MAT2X2F tmp = gr_mat2x2f_zero();

    if (a && b) {
        tmp.m00 = a->m00 * b->m00 + a->m01 * b->m10;
        tmp.m01 = a->m00 * b->m01 + a->m01 * b->m11;
        tmp.m10 = a->m10 * b->m00 + a->m11 * b->m10;
        tmp.m11 = a->m10 * b->m01 + a->m11 * b->m11;
    }

    return tmp;
}

GR_MAT2X2F gr_mat2x2f_mul_real(const GR_MAT2X2F *a, float b)
{
    GR_MAT2X2F tmp = gr_mat2x2f_zero();

    if (a) {
        tmp.m00 = a->m00 * b;
        tmp.m01 = a->m01 * b;
        tmp.m10 = a->m10 * b;
        tmp.m11 = a->m11 * b;
    }

    return tmp;
}

GR_VEC2F gr_mat2x2f_mul_vec(const GR_MAT2X2F *a, const GR_VEC2F *b)
{
    GR_VEC2F tmp = { 0 };

    if (a && b) {
        tmp.x = a->m00 * b->x + a->m01 * b->y;
        tmp.y = a->m10 * b->x + a->m11 * b->y;
    }

    return tmp;
}

/*===========================================================================*/
