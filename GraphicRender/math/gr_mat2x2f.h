#ifndef __GR_MAT2X2F_H__
#define __GR_MAT2X2F_H__

#include "gr_vec2f.h"

/*===========================================================================*/
/* 2x2 方阵，行主序存储 */
/*===========================================================================*/

typedef union {
    struct {
        float m00;
        float m01;
        float m10;
        float m11;
    };

    float mat[4];
    float matij[2][2];
} GR_MAT2X2F;

/* 构建单位矩阵 */
GR_MAT2X2F gr_mat2x2f_unit();

/* 构建零矩阵 */
GR_MAT2X2F gr_mat2x2f_zero();

/* 构建矩阵 */
GR_MAT2X2F gr_mat2x2f_array(const float *elems);

/* 构建矩阵 */
GR_MAT2X2F gr_mat2x2f_array2d(const float (*elems)[2]);

/* 转置矩阵 */
GR_MAT2X2F gr_mat2x2f_transpose(const GR_MAT2X2F *mat);

/* 逆矩阵 */
GR_MAT2X2F gr_mat2x2f_inverse(const GR_MAT2X2F *mat);

/* 2 x 2 行列式值 */
float gr_mat2x2f_determinant(const GR_MAT2X2F *mat); 

/* 矩阵加 */
GR_MAT2X2F gr_mat2x2f_add(const GR_MAT2X2F *a, const GR_MAT2X2F *b);

/* 矩阵减 */
GR_MAT2X2F gr_mat2x2f_sub(const GR_MAT2X2F *a, const GR_MAT2X2F *b);

/* 矩阵乘 */
GR_MAT2X2F gr_mat2x2f_mul(const GR_MAT2X2F *a, const GR_MAT2X2F *b);

/* 矩阵和标量乘 */
GR_MAT2X2F gr_mat2x2f_mul_real(const GR_MAT2X2F *a, float b);

/* 矩阵和向量乘 */
GR_VEC2F gr_mat2x2f_mul_vec(const GR_MAT2X2F *a, const GR_VEC2F *b);

/*===========================================================================*/

#endif /* __GR_MAT2X2F_H__ */
