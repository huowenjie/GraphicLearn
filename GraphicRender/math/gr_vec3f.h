#ifndef __GR_VEC3F_H__
#define __GR_VEC3F_H__

#include "../gr_type.h"

/*===========================================================================*/
/* 三维向量 */
/*===========================================================================*/

typedef struct gr_vec3f
{
    float x;
    float y;
    float z;
} GR_VEC3F;

/**
 * 向量运算
 *
 * gr_vec3f_add 向量加
 * gr_vec3f_sub 向量减
 * gr_vec3f_mul 向量与实数相乘
 * gr_vec3f_div 向量与实数相除
 * gr_vec3f_reverse 向量取反
 * gr_vec3f_normalize 向量归一化
 * gr_vec3f_lerp 向量线性插值, p = (1 - t) * a + t * b 当 t = 0 时返回 a，否则返回 b
 * 
 * gr_vec3f_dot 向量点乘
 * gr_vec3f_cross 向量叉乘
 * gr_vec3f_length 求向量的长度
 * gr_vec3f_angle 求两个向量的夹角
 * 
 * gr_vec3f_line_comb 向量的线性组合，vlist 为向量列表，wlist 为权列表，k 为项数
 */

GR_VEC3F gr_vec3f_add(const GR_VEC3F *a, const GR_VEC3F *b);
GR_VEC3F gr_vec3f_sub(const GR_VEC3F *a, const GR_VEC3F *b);
GR_VEC3F gr_vec3f_mul(const GR_VEC3F *a, float t);
GR_VEC3F gr_vec3f_div(const GR_VEC3F *a, float t);
GR_VEC3F gr_vec3f_reverse(const GR_VEC3F *a);
GR_VEC3F gr_vec3f_normalize(const GR_VEC3F *a);

GR_VEC3F gr_vec3f_lerp(const GR_VEC3F *a, const GR_VEC3F *b, float t);

float gr_vec3f_dot(const GR_VEC3F *a, const GR_VEC3F *b);
GR_VEC3F gr_vec3f_cross(const GR_VEC3F *a, const GR_VEC3F *b);
float gr_vec3f_length(const GR_VEC3F *a);
float gr_vec3f_angle(const GR_VEC3F *a, const GR_VEC3F *b);

GR_VEC3F gr_vec3f_line_comb(const GR_VEC3F *vlist, const float *wlist, int k);

/* 基于两个向量构建正交基 */
GR_BOOL gr_vec3f_build_basic(
    const GR_VEC3F *a,
    const GR_VEC3F *b,
    GR_VEC3F *w,
    GR_VEC3F *u,
    GR_VEC3F *v);

/*===========================================================================*/

#endif /* __GR_VEC3F_H__ */
