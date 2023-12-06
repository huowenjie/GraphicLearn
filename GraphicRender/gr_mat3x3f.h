#ifndef __GR_MAT3X3F_H__
#define __GR_MAT3X3F_H__

#include "gr_vec3f.h"

/*===========================================================================*/
/* 3x3 方阵，行主序存储 */
/*===========================================================================*/

typedef union {
    /**
     * [ m00, m01, m02 ]
     * [ m10, m11, m12 ]
     * [ m20, m21, m22 ]
     */
    struct {
        float m00;
        float m01;
        float m02;
        float m10;
        float m11;
        float m12;
        float m20;
        float m21;
        float m22;
    };

    float mat[9];
    float matij[3][3];
} GR_MAT3X3F;

/* 构建单位矩阵 */
GR_MAT3X3F gr_mat3x3f_unit();

/* 构建零矩阵 */
GR_MAT3X3F gr_mat3x3f_zero();

/* 构建矩阵 */
GR_MAT3X3F gr_mat3x3f_array(const float *elems);

/* 构建矩阵 */
GR_MAT3X3F gr_mat3x3f_array2d(const float (*elems)[3]);

/* 转置矩阵 */
GR_MAT3X3F gr_mat3x3f_transpose(const GR_MAT3X3F *mat);

/* 逆矩阵 */
GR_MAT3X3F gr_mat3x3f_inverse(const GR_MAT3X3F *mat);

/* 3 x 3 行列式值 */
float gr_mat3x3f_determinant(const GR_MAT3X3F *mat); 

/* 矩阵加 */
GR_MAT3X3F gr_mat3x3f_add(const GR_MAT3X3F *a, const GR_MAT3X3F *b);

/* 矩阵减 */
GR_MAT3X3F gr_mat3x3f_sub(const GR_MAT3X3F *a, const GR_MAT3X3F *b);

/* 矩阵乘 */
GR_MAT3X3F gr_mat3x3f_mul(const GR_MAT3X3F *a, const GR_MAT3X3F *b);

/* 矩阵和标量乘 */
GR_MAT3X3F gr_mat3x3f_mul_real(const GR_MAT3X3F *a, float b);

/* 矩阵和向量乘 */
GR_VEC3F gr_mat3x3f_mul_vec(const GR_MAT3X3F *a, const GR_VEC3F *b);

/*===========================================================================*/

#endif /* __GR_MAT3X3F_H__ */
