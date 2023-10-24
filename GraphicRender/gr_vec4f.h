#ifndef __GR_VEC4F_H__
#define __GR_VEC4F_H__

#include "gr_type.h"

/*===========================================================================*/
/* 三维向量 */
/*===========================================================================*/

typedef struct gr_vec4f
{
    float x;
    float y;
    float z;
    float w;
} GR_VEC4F;

/**
 * 向量运算
 *
 * gr_vec4f_add 向量加
 * gr_vec4f_sub 向量减
 * gr_vec4f_mul 向量与实数相乘
 * gr_vec4f_div 向量与实数相除
 * gr_vec4f_reverse 向量取反
 * gr_vec4f_normalize 向量归一化
 * gr_vec4f_lerp 向量线性插值, p = (1 - t) * a + t * b 当 t = 0 时返回 a，否则返回 b
 * 
 * gr_vec4f_dot 向量点乘
 * gr_vec4f_cross 向量叉乘的模，在 2 维平面上，我们忽略向量叉乘后的方向，仅计算其模
 * gr_vec4f_length 求向量的长度
 * gr_vec4f_angle 求两个向量的夹角
 */
GR_VEC4F gr_vec4f_add(const GR_VEC4F *a, const GR_VEC4F *b);
GR_VEC4F gr_vec4f_sub(const GR_VEC4F *a, const GR_VEC4F *b);
GR_VEC4F gr_vec4f_mul(const GR_VEC4F *a, float t);
GR_VEC4F gr_vec4f_div(const GR_VEC4F *a, float t);
GR_VEC4F gr_vec4f_reverse(const GR_VEC4F *a);
GR_VEC4F gr_vec4f_normalize(const GR_VEC4F *a);

GR_VEC4F gr_vec4f_lerp(const GR_VEC4F *a, const GR_VEC4F *b, float t);

float gr_vec4f_dot(const GR_VEC4F *a, const GR_VEC4F *b);
float gr_vec4f_cross(const GR_VEC4F *a, const GR_VEC4F *b);
float gr_vec4f_length(const GR_VEC4F *a);
float gr_vec4f_angle(const GR_VEC4F *a, const GR_VEC4F *b);

/*===========================================================================*/

#endif /* __GR_VEC4F_H__ */
