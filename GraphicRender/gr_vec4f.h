#ifndef __GR_VEC4F_H__
#define __GR_VEC4F_H__

#include "gr_type.h"
#include "gr_vec3f.h"

/*===========================================================================*/
/* 三维齐次向量/四维向量 */
/*===========================================================================*/

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} GR_VEC4F;

/* 构建向量 */
GR_VEC4F gr_vec4f_xyzw(float x, float y, float z, float w);

/* 构建向量 */
GR_VEC4F gr_vec4f_vec3f(const GR_VEC3F *v, float w);

/* 向量加 */
GR_VEC4F gr_vec4f_add(const GR_VEC4F *a, const GR_VEC4F *b);

/* 向量减 */
GR_VEC4F gr_vec4f_sub(const GR_VEC4F *a, const GR_VEC4F *b);

/* 向量乘 */
GR_VEC4F gr_vec4f_mul(const GR_VEC4F *a, float t);

/* 向量除以整数 */
GR_VEC4F gr_vec4f_div(const GR_VEC4F *a, float t);

/* 向量取反 */
GR_VEC4F gr_vec4f_reverse(const GR_VEC4F *a);

/* 向量线性插值 p = (1 - t) * a + t * b 当 t = 0 时返回 a，否则返回 b */
GR_VEC4F gr_vec4f_lerp(const GR_VEC4F *a, const GR_VEC4F *b, float t);

/* 向量线性组合 vlist 为向量列表，wlist 为权列表，k 为项数 */
GR_VEC4F gr_vec4f_line_comb(const GR_VEC4F *vlist, const float *wlist, int k);

/* 4d 向量齐次除法 */
GR_VEC4F gr_vec4f_homogen_div(const GR_VEC4F *a);

/*===========================================================================*/

#endif /* __GR_VEC4F_H__ */
