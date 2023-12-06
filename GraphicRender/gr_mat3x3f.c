#include <math.h>
#include "gr_mat3x3f.h"

/*===========================================================================*/

GR_MAT3X3F gr_mat3x3f_unit()
{
    GR_MAT3X3F mat;

    mat.m00 = 1.0f;
    mat.m01 = 0.0f;
    mat.m02 = 0.0f;
    mat.m10 = 0.0f;
    mat.m11 = 1.0f;
    mat.m12 = 0.0f;
    mat.m20 = 0.0f;
    mat.m21 = 0.0f;
    mat.m22 = 1.0f;
    return mat;
}

GR_MAT3X3F gr_mat3x3f_zero()
{
    GR_MAT3X3F mat;

    mat.m00 = 0.0f;
    mat.m01 = 0.0f;
    mat.m02 = 0.0f;
    mat.m10 = 0.0f;
    mat.m11 = 0.0f;
    mat.m12 = 0.0f;
    mat.m20 = 0.0f;
    mat.m21 = 0.0f;
    mat.m22 = 0.0f;
    return mat;
}

GR_MAT3X3F gr_mat3x3f_array(const float *elems)
{
    GR_MAT3X3F mat = gr_mat3x3f_zero();

    if (elems) {
        mat.m00 = elems[0];
        mat.m01 = elems[1];
        mat.m02 = elems[2];
        mat.m10 = elems[3];
        mat.m11 = elems[4];
        mat.m12 = elems[5];
        mat.m20 = elems[6];
        mat.m21 = elems[7];
        mat.m22 = elems[8];
    }
    return mat;
}

GR_MAT3X3F gr_mat3x3f_array2d(const float (*elems)[3])
{
    GR_MAT3X3F mat = gr_mat3x3f_zero();

    if (elems) {
        mat.m00 = elems[0][0];
        mat.m01 = elems[0][1];
        mat.m02 = elems[0][2];
        mat.m10 = elems[1][0];
        mat.m11 = elems[1][1];
        mat.m12 = elems[1][2];
        mat.m20 = elems[2][0];
        mat.m21 = elems[2][1];
        mat.m22 = elems[2][2];
    }
    return mat;
}

GR_MAT3X3F gr_mat3x3f_transpose(const GR_MAT3X3F *mat)
{
    GR_MAT3X3F tmp = gr_mat3x3f_zero();

    if (mat) {
        tmp = *mat;
        tmp.m01 = mat->m10;
        tmp.m10 = mat->m01;
        tmp.m02 = mat->m20;
        tmp.m20 = mat->m02;
        tmp.m12 = mat->m21;
        tmp.m21 = mat->m12;
    }
    return tmp;
}

GR_MAT3X3F gr_mat3x3f_inverse(const GR_MAT3X3F *mat)
{
    GR_MAT3X3F tmp = gr_mat3x3f_zero();
    float det = 0.0f;

    if (!mat) {
        return tmp;
    }

    /* 求行列式 */
    det = gr_mat3x3f_determinant(mat);

    /* 行列式为 0，不可逆，直接返回原矩阵 */
    if (det == 0.0f || fabsf(det) < 1e-6f) {
        return *mat;
    }

    /*
     * 利用克拉默法则来求解逆矩阵
     * inverse A = adj A / det A
     */
    tmp.m00 = (mat->m11 * mat->m22 - mat->m12 * mat->m21) / det;
    tmp.m01 = (mat->m02 * mat->m21 - mat->m01 * mat->m22) / det;
    tmp.m02 = (mat->m01 * mat->m12 - mat->m02 * mat->m11) / det;

    tmp.m10 = (mat->m12 * mat->m20 - mat->m10 * mat->m22) / det;
    tmp.m11 = (mat->m00 * mat->m22 - mat->m02 * mat->m20) / det;
    tmp.m12 = (mat->m02 * mat->m10 - mat->m00 * mat->m12) / det;

    tmp.m20 = (mat->m10 * mat->m21 - mat->m11 * mat->m20) / det;
    tmp.m21 = (mat->m01 * mat->m20 - mat->m00 * mat->m21) / det;
    tmp.m22 = (mat->m00 * mat->m11 - mat->m01 * mat->m10) / det;

    return tmp;
}

float gr_mat3x3f_determinant(const GR_MAT3X3F *mat)
{
    float det = 0.0f;

    if (mat) {
        det = 
            mat->m00 * mat->m11 * mat->m22 +
            mat->m02 * mat->m10 * mat->m21 +
            mat->m01 * mat->m12 * mat->m20 -
            mat->m00 * mat->m21 * mat->m12 - 
            mat->m01 * mat->m10 * mat->m22 -
            mat->m02 * mat->m11 * mat->m20;
    }
    return det;
}

GR_MAT3X3F gr_mat3x3f_add(const GR_MAT3X3F *a, const GR_MAT3X3F *b)
{
    GR_MAT3X3F tmp = gr_mat3x3f_zero();

    if (a && b) {
        tmp.m00 = a->m00 + b->m00;
        tmp.m01 = a->m01 + b->m01;
        tmp.m02 = a->m02 + b->m02;
        tmp.m10 = a->m10 + b->m10;
        tmp.m11 = a->m11 + b->m11;
        tmp.m12 = a->m12 + b->m12;
        tmp.m20 = a->m20 + b->m20;
        tmp.m21 = a->m21 + b->m21;
        tmp.m22 = a->m22 + b->m22;
    }
    return tmp;
}

GR_MAT3X3F gr_mat3x3f_sub(const GR_MAT3X3F *a, const GR_MAT3X3F *b)
{
    GR_MAT3X3F tmp = gr_mat3x3f_zero();

    if (a && b) {
        tmp.m00 = a->m00 - b->m00;
        tmp.m01 = a->m01 - b->m01;
        tmp.m02 = a->m02 - b->m02;
        tmp.m10 = a->m10 - b->m10;
        tmp.m11 = a->m11 - b->m11;
        tmp.m12 = a->m12 - b->m12;
        tmp.m20 = a->m20 - b->m20;
        tmp.m21 = a->m21 - b->m21;
        tmp.m22 = a->m22 - b->m22;
    }
    return tmp;
}

GR_MAT3X3F gr_mat3x3f_mul(const GR_MAT3X3F *a, const GR_MAT3X3F *b)
{
    GR_MAT3X3F tmp = gr_mat3x3f_zero();

    if (a && b) {
        tmp.m00 = a->m00 * b->m00 + a->m01 * b->m10 + a->m02 * b->m20;
        tmp.m01 = a->m00 * b->m01 + a->m01 * b->m11 + a->m02 * b->m21;
        tmp.m02 = a->m00 * b->m02 + a->m01 * b->m12 + a->m02 * b->m22;
        tmp.m10 = a->m10 * b->m00 + a->m11 * b->m10 + a->m12 * b->m20;
        tmp.m11 = a->m10 * b->m01 + a->m11 * b->m11 + a->m12 * b->m21;
        tmp.m12 = a->m10 * b->m02 + a->m11 * b->m12 + a->m12 * b->m22;
        tmp.m20 = a->m20 * b->m00 + a->m21 * b->m10 + a->m22 * b->m20;
        tmp.m21 = a->m20 * b->m01 + a->m21 * b->m11 + a->m22 * b->m21;
        tmp.m22 = a->m20 * b->m02 + a->m21 * b->m12 + a->m22 * b->m22;
    }
    return tmp;
}

GR_MAT3X3F gr_mat3x3f_mul_real(const GR_MAT3X3F *a, float b)
{
    GR_MAT3X3F tmp = gr_mat3x3f_zero();

    if (a) {
        tmp.m00 = a->m00 * b;
        tmp.m01 = a->m01 * b;
        tmp.m02 = a->m02 * b;
        tmp.m10 = a->m10 * b;
        tmp.m11 = a->m11 * b;
        tmp.m12 = a->m12 * b;
        tmp.m20 = a->m20 * b;
        tmp.m21 = a->m21 * b;
        tmp.m22 = a->m22 * b;
    }
    return tmp;
}

GR_VEC3F gr_mat3x3f_mul_vec(const GR_MAT3X3F *a, const GR_VEC3F *b)
{
    GR_VEC3F tmp = { 0 };

    if (a && b) {
        tmp.x = a->m00 * b->x + a->m01 * b->y + a->m02 * b->z;
        tmp.y = a->m10 * b->x + a->m11 * b->y + a->m12 * b->z;
        tmp.z = a->m20 * b->x + a->m21 * b->y + a->m22 * b->z;
    }

    return tmp;
}

/*===========================================================================*/
