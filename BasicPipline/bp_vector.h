#ifndef BP_VECTOR_H
#define BP_VECTOR_H

/*===========================================================================*/
/* 向量 */
/*===========================================================================*/

/* 向量 */
struct vec4f {
    float x;
    float y;
    float z;
    float w;
};

/* 带颜色的顶点 */
struct vec4f_point {
    float x;
    float y;
    float z;
    float w;

    float r;
    float g;
    float b;
    float a;
};

struct vec4f *vec4f_zero(struct vec4f *v);
struct vec4f *vec4f_zero_pt(struct vec4f *v);

/* 取相反向量 */
struct vec4f *vec4f_opposite(const struct vec4f *v, struct vec4f *ret);

/* 向量点乘 */
float vec4f_dot(const struct vec4f *v1, const struct vec4f *v2);

/* 向量叉乘 */
struct vec4f *vec4f_cross(const struct vec4f *v1, const struct vec4f *v2, struct vec4f *ret);

/* 向量归一化 */
struct vec4f *vec4f_normalize(struct vec4f *v);

/* 点齐次化 */
struct vec4f *vec4f_homogeneous(struct vec4f *v);

/* 向量加 */
struct vec4f *vec4f_add(const struct vec4f *v1, const struct vec4f *v2, struct vec4f *ret);

/* 向量减*/
struct vec4f *vec4f_sub(const struct vec4f *v1, const struct vec4f *v2, struct vec4f *ret);

/* 向量乘 */
struct vec4f *vec4f_mult(const struct vec4f *v, float n, struct vec4f *ret);

/* 求法向量(结果进行归一化) */
struct vec4f *vec4f_normal(const struct vec4f *v1, const struct vec4f *v2, struct vec4f *normal);

/*===========================================================================*/

#endif /* BP_VECTOR_H */
