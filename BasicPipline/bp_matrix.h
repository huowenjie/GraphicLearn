#ifndef BP_MATRIX_H
#define BP_MATRIX_H

/*===========================================================================*/
/* 矩阵 */
/*===========================================================================*/

#include "bp_vector.h"

/* 4x4矩阵及变换 , 行主序 */
struct mat4x4f {
	union {
		float mat[4][4];
		struct {
			float m00, m01, m02, m03,
				  m10, m11, m12, m13,
				  m20, m21, m22, m23,
				  m30, m31, m32, m33;
		};
	};
};

/* 单位矩阵 */
struct mat4x4f *mat4x4f_unit(struct mat4x4f *m);

/* 矩阵变换 */
struct vec4f *mat4x4f_mult2vec(const struct mat4x4f *m, const struct vec4f *v, struct vec4f *ret);

/* 矩阵乘法 */
struct mat4x4f *mat4x4f_mult(const struct mat4x4f *ma, const struct mat4x4f *mb, struct mat4x4f *ret);

/* 平移变换 */
struct mat4x4f *mat4x4f_translate(struct mat4x4f *mat, const struct vec4f *vec);

/* 旋转变换 */
struct mat4x4f *mat4x4f_rotate(struct mat4x4f *mat, float radians, const struct vec4f *axis);

/* 缩放变换 */
struct mat4x4f *mat4x4f_scale(struct mat4x4f *mat, float sx, float sy, float sz);

/* 投影矩阵 */
struct mat4x4f *mat4x4f_projection(
	struct mat4x4f *mat, 
	const struct vec4f *from_min,
	const struct vec4f *from_max,
	const struct vec4f *to_min,
	const struct vec4f *to_max
);

/** 
 * 透视投影变换
 * 
 * fov -- 纵向视角，一般设置为 PI / 4
 * aspect -- 裁剪面 高度/宽度
 * near -- 近平面距摄像机距离，一般设置为 0.1f
 * far -- 远平面距摄像机距离，一般设置为 100.0f
 */
struct mat4x4f *mat4x4f_perspective(
	struct mat4x4f *mat,
    float fov,
    float aspect,
    float near,
    float far
);

/* 摄像机视角变换 */
struct mat4x4f *mat4x4f_lookat(
	struct mat4x4f *mat,
    const struct vec4f *pos,
    const struct vec4f *target,
    const struct vec4f *up
);

/*===========================================================================*/

#endif /* BP_MATRIX_H */
