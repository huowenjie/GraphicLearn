#ifndef __GR_VEC3F_H__
#define __GR_VEC3F_H__

#include "gr_type.h"
#include "gr_vec2f.h"

/*===========================================================================*/
/* 三维向量 */
/*===========================================================================*/

typedef struct
{
    float x;
    float y;
    float z;
} GR_VEC3F;

/* 构建向量 */
GR_VEC3F gr_vec3f_xyz(float x, float y, float z);

/* 构建向量 */
GR_VEC3F gr_vec3f_vec2f(const GR_VEC2F *v, float z);

/* 向量加 */
GR_VEC3F gr_vec3f_add(const GR_VEC3F *a, const GR_VEC3F *b);

/* 向量减 */
GR_VEC3F gr_vec3f_sub(const GR_VEC3F *a, const GR_VEC3F *b);

/* 向量与实数相乘 */
GR_VEC3F gr_vec3f_mul(const GR_VEC3F *a, float t);

/* 向量与实数相除 */
GR_VEC3F gr_vec3f_div(const GR_VEC3F *a, float t);

/* 向量取反 */
GR_VEC3F gr_vec3f_reverse(const GR_VEC3F *a);

/* 向量归一化 */
GR_VEC3F gr_vec3f_normalize(const GR_VEC3F *a);

/* 向量线性插值, p = (1 - t) * a + t * b 当 t = 0 时返回 a，否则返回 b */
GR_VEC3F gr_vec3f_lerp(const GR_VEC3F *a, const GR_VEC3F *b, float t);

/* 向量点乘 */
float gr_vec3f_dot(const GR_VEC3F *a, const GR_VEC3F *b);

/* 向量叉乘 */
GR_VEC3F gr_vec3f_cross(const GR_VEC3F *a, const GR_VEC3F *b);

/* 求向量的长度 */
float gr_vec3f_length(const GR_VEC3F *a);

/* 求两个向量的夹角 */
float gr_vec3f_angle(const GR_VEC3F *a, const GR_VEC3F *b);

/* 向量的线性组合，vlist 为向量列表，wlist 为权列表，k 为项数 */
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
