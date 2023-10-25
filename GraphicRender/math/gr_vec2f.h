#ifndef __GR_VEC2F_H__
#define __GR_VEC2F_H__

#include "gr_type.h"

/*===========================================================================*/
/* 二维向量 */
/*===========================================================================*/

typedef struct gr_vec2f
{
    float x;
    float y;
} GR_VEC2F;

/**
 * 向量运算
 *
 * gr_vec2f_add 向量加
 * gr_vec2f_sub 向量减
 * gr_vec2f_mul 向量与实数相乘
 * gr_vec2f_div 向量与实数相除
 * gr_vec2f_reverse 向量取反
 * gr_vec2f_normalize 向量归一化
 * gr_vec2f_lerp 向量线性插值, p = (1 - t) * a + t * b 当 t = 0 时返回 a，否则返回 b
 * 
 * gr_vec2f_dot 向量点乘
 * gr_vec2f_cross 向量叉乘的模，在 2 维平面上，我们忽略向量叉乘后的方向，仅计算其模
 * gr_vec2f_length 求向量的长度
 * gr_vec2f_angle 求两个向量的夹角
 * 
 * gr_vec2f_line_comb 向量的线性组合，vlist 为向量列表，wlist 为权列表，k 为项数
 */

GR_VEC2F gr_vec2f_add(const GR_VEC2F *a, const GR_VEC2F *b);
GR_VEC2F gr_vec2f_sub(const GR_VEC2F *a, const GR_VEC2F *b);
GR_VEC2F gr_vec2f_mul(const GR_VEC2F *a, float t);
GR_VEC2F gr_vec2f_div(const GR_VEC2F *a, float t);
GR_VEC2F gr_vec2f_reverse(const GR_VEC2F *a);
GR_VEC2F gr_vec2f_normalize(const GR_VEC2F *a);

GR_VEC2F gr_vec2f_lerp(const GR_VEC2F *a, const GR_VEC2F *b, float t);

float gr_vec2f_dot(const GR_VEC2F *a, const GR_VEC2F *b);
float gr_vec2f_cross(const GR_VEC2F *a, const GR_VEC2F *b);
float gr_vec2f_length(const GR_VEC2F *a);
float gr_vec2f_angle(const GR_VEC2F *a, const GR_VEC2F *b);

GR_VEC2F gr_vec2f_line_comb(const GR_VEC2F *vlist, const float *wlist, int k);

/*===========================================================================*/

#endif /* __GR_VEC2F_H__ */
