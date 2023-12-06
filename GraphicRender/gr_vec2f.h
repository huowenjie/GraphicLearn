#ifndef __GR_VEC2F_H__
#define __GR_VEC2F_H__

#include "gr_type.h"

/*===========================================================================*/
/* 二维向量 */
/*===========================================================================*/

typedef struct
{
    float x;
    float y;
} GR_VEC2F;

/* 构建向量 */
GR_VEC2F gr_vec2f_xy(float x, float y);

/* 向量加 */
GR_VEC2F gr_vec2f_add(const GR_VEC2F *a, const GR_VEC2F *b);

/* 向量减 */
GR_VEC2F gr_vec2f_sub(const GR_VEC2F *a, const GR_VEC2F *b);

/* 向量与实数相乘 */
GR_VEC2F gr_vec2f_mul(const GR_VEC2F *a, float t);

/* 向量与实数相除 */
GR_VEC2F gr_vec2f_div(const GR_VEC2F *a, float t);

/* 向量取反 */
GR_VEC2F gr_vec2f_reverse(const GR_VEC2F *a);

/* 向量归一化 */
GR_VEC2F gr_vec2f_normalize(const GR_VEC2F *a);

/* 向量线性插值, p = (1 - t) * a + t * b 当 t = 0 时返回 a，否则返回 b */
GR_VEC2F gr_vec2f_lerp(const GR_VEC2F *a, const GR_VEC2F *b, float t);

/* 向量点乘 */
float gr_vec2f_dot(const GR_VEC2F *a, const GR_VEC2F *b);

/* 向量叉乘的模，在 2 维平面上，我们忽略向量叉乘后的方向，仅计算其模 */
float gr_vec2f_cross(const GR_VEC2F *a, const GR_VEC2F *b);

/* 求向量的长度 */
float gr_vec2f_length(const GR_VEC2F *a);

/* 求两个向量的夹角 */
float gr_vec2f_angle(const GR_VEC2F *a, const GR_VEC2F *b);

/* 向量的线性组合，vlist 为向量列表，wlist 为权列表，k 为项数 */
GR_VEC2F gr_vec2f_line_comb(const GR_VEC2F *vlist, const float *wlist, int k);

/* 判断点 p 是否在三角形 abc 内 */
GR_BOOL gr_vec2f_inside_triangle(
    const GR_VEC2F *a,
    const GR_VEC2F *b,
    const GR_VEC2F *c,
    const GR_VEC2F *p);

/*===========================================================================*/

#endif /* __GR_VEC2F_H__ */
