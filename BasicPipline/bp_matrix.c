#include <math.h>
#include <stddef.h>
#include "bp_matrix.h"

/*===========================================================================*/

#define ZERO_RANGE_FLOAT (1e-6f)

#define MAT4X4F_SUB_MULT(i, j, m1, m2) \
	m1->m##i##0 * m2->m##0##j + \
	m1->m##i##1 * m2->m##1##j + \
	m1->m##i##2 * m2->m##2##j + \
	m1->m##i##3 * m2->m##3##j

struct mat4x4f *mat4x4f_unit(struct mat4x4f *m) 
{
	if (m) {
		m->m00 = m->m11 = m->m22 = m->m33 = 1.0f;
		m->m01 = m->m10 = m->m20 = m->m30 = 0.0f;
		m->m02 = m->m12 = m->m21 = m->m31 = 0.0f;
		m->m03 = m->m13 = m->m23 = m->m32 = 0.0f;
	}
	return NULL;
}

struct vec4f *mat4x4f_mult2vec(const struct mat4x4f *m, const struct vec4f *v, struct vec4f *ret)
{
	if (m && v && ret) {
		struct vec4f tmp;

		tmp.x = m->m00 * v->x + m->m01 * v->y + m->m02 * v->z + m->m03 * v->w;
		tmp.y = m->m10 * v->x + m->m11 * v->y + m->m12 * v->z + m->m13 * v->w;
		tmp.z = m->m20 * v->x + m->m21 * v->y + m->m22 * v->z + m->m23 * v->w;
		tmp.w = m->m30 * v->x + m->m31 * v->y + m->m32 * v->z + m->m33 * v->w;

		*ret = tmp;
		return ret;
	}
	return NULL;
}

struct mat4x4f *mat4x4f_mult(const struct mat4x4f *ma, const struct mat4x4f *mb, struct mat4x4f *ret)
{
	struct mat4x4f tmp;

	if (!ma || !mb || !ret) {
		return NULL;
	}

	mat4x4f_unit(&tmp);

	tmp.m00 = MAT4X4F_SUB_MULT(0, 0, ma, mb);
	tmp.m01 = MAT4X4F_SUB_MULT(0, 1, ma, mb);
	tmp.m02 = MAT4X4F_SUB_MULT(0, 2, ma, mb);
	tmp.m03 = MAT4X4F_SUB_MULT(0, 3, ma, mb);
	tmp.m10 = MAT4X4F_SUB_MULT(1, 0, ma, mb);
	tmp.m11 = MAT4X4F_SUB_MULT(1, 1, ma, mb);
	tmp.m12 = MAT4X4F_SUB_MULT(1, 2, ma, mb);
	tmp.m13 = MAT4X4F_SUB_MULT(1, 3, ma, mb);
	tmp.m20 = MAT4X4F_SUB_MULT(2, 0, ma, mb);
	tmp.m21 = MAT4X4F_SUB_MULT(2, 1, ma, mb);
	tmp.m22 = MAT4X4F_SUB_MULT(2, 2, ma, mb);
	tmp.m23 = MAT4X4F_SUB_MULT(2, 3, ma, mb);
	tmp.m30 = MAT4X4F_SUB_MULT(3, 0, ma, mb);
	tmp.m31 = MAT4X4F_SUB_MULT(3, 1, ma, mb);
	tmp.m32 = MAT4X4F_SUB_MULT(3, 2, ma, mb);
	tmp.m33 = MAT4X4F_SUB_MULT(3, 3, ma, mb);

	*ret = tmp;
	return ret;
}

struct mat4x4f *mat4x4f_translate(struct mat4x4f *mat, const struct vec4f *vec)
{
	struct mat4x4f tmp;

	if (!mat || !vec) {
		return NULL;
	}

	mat4x4f_unit(&tmp);

	tmp.m03 = vec->x;
	tmp.m13 = vec->y;
	tmp.m23 = vec->z;
	tmp.m33 = 1.0f;

	mat4x4f_mult(&tmp, mat, mat);
	return mat;
}

struct mat4x4f *mat4x4f_rotate(struct mat4x4f *mat, float radians, const struct vec4f *axis)
{
	struct mat4x4f tmp;

	float cos_theta = 0.0f;
	float sin_theta = 0.0f;
	float one_theta = 0.0f;
	float xy = 0.0f;
	float xz = 0.0f;
	float yz = 0.0f;

	if (!mat || !axis) {
		return NULL;
	}

	mat4x4f_unit(&tmp);

	cos_theta = cosf(radians);
	sin_theta = sinf(radians);
	one_theta = 1.0f - cos_theta;

	xy = axis->x * axis->y;
	xz = axis->x * axis->z;
	yz = axis->y * axis->z;

	tmp.m00 = axis->x * axis->x * one_theta + cos_theta;
	tmp.m01 = xy * one_theta - axis->z * sin_theta;
	tmp.m02 = xz * one_theta + axis->y * sin_theta;

	tmp.m10 = xy * one_theta + axis->z * sin_theta;
	tmp.m11 = axis->y * axis->y * one_theta + cos_theta;
	tmp.m12 = yz * one_theta - axis->x * sin_theta;

	tmp.m20 = xz * one_theta - axis->y * sin_theta;
	tmp.m21 = yz * one_theta + axis->x * sin_theta;
	tmp.m22 = axis->z * axis->z * one_theta + cos_theta;

	mat4x4f_mult(&tmp, mat, mat);
	return mat;
}

struct mat4x4f *mat4x4f_scale(struct mat4x4f *mat, float sx, float sy, float sz)
{
	struct mat4x4f tmp;

	if (!mat) {
		return NULL;
	}

	mat4x4f_unit(&tmp);

	tmp.m00 = sx;
	tmp.m11 = sy;
	tmp.m22 = sz;
	tmp.m33 = 1.0f;

	mat4x4f_mult(&tmp, mat, mat);
	return mat;
}

struct mat4x4f *mat4x4f_projection(
	struct mat4x4f *mat,
	const struct vec4f *from_min,
	const struct vec4f *from_max,
	const struct vec4f *to_min,
	const struct vec4f *to_max)
{
	struct mat4x4f tmp;
	float dx = 0.0f;
	float dy = 0.0f;
	float dz = 0.0f;

	if (!mat || !from_min || !from_max ||
		!to_min || !to_max) {
		return NULL;
	}

	dx = from_max->x - from_min->x;
	if (dx < 0.0f || (fabsf(dx) < ZERO_RANGE_FLOAT)) {
		return NULL;
	}

	dy = from_max->y - from_min->y;
	if (dy < 0.0f || (fabsf(dy) < ZERO_RANGE_FLOAT)) {
		return NULL;
	}

	dz = from_max->z - from_min->z;
	if (dz < 0.0f || (fabsf(dz) < ZERO_RANGE_FLOAT)) {
		return NULL;
	}

	mat4x4f_unit(&tmp);

	tmp.m00 = (to_max->x - to_min->x) / dx;
	tmp.m11 = (to_max->y - to_min->y) / dy;
	tmp.m22 = (to_max->z - to_min->z) / dz;

	tmp.m03 = (from_max->x * to_min->x - from_min->x * to_max->x) / dx;
	tmp.m13 = (from_max->y * to_min->y - from_min->y * to_max->y) / dy;
	tmp.m23 = (from_max->z * to_min->z - from_min->z * to_max->z) / dz;
	tmp.m33 = 1.0f;

	mat4x4f_mult(&tmp, mat, mat);
	return mat;
}

struct mat4x4f *mat4x4f_perspective(
	struct mat4x4f *mat, float fov, float aspect, float near, float far) 
{
	struct mat4x4f tmp;

	if (!mat || fabsf(fov) < ZERO_RANGE_FLOAT || fabsf(aspect) < ZERO_RANGE_FLOAT) {
		return NULL;
	}

	if (fabsf(near - far) < ZERO_RANGE_FLOAT) {
		return NULL;
	}

	mat4x4f_unit(&tmp);

	tmp.m00 = 1.0f / (tanf(fov) * aspect);
	tmp.m11 = 1.0f / tanf(fov);
	tmp.m22 = (near + far) / (near - far);
	tmp.m23 = 2.0f * near * far / (near - far);
	tmp.m32 = -1.0f;
	tmp.m33 = 0.0f;

	mat4x4f_mult(&tmp, mat, mat);
	return mat;
}

#if 0
struct mat4x4f *mat4x4f_perspective_ex(
	struct mat4x4f *mat,
    float fov,
    float aspect,
    float near,
    float far
)
{
    struct mat4x4f tmp;
    float r = 0.0f;
    float l = 0.0f;
    float t = 0.0f;
    float b = 0.0f;

	if (!mat) {
		return NULL;
	}

	if (!mat || fabsf(fov) < ZERO_RANGE_FLOAT || fabsf(aspect) < ZERO_RANGE_FLOAT) {
		return NULL;
	}

	if (fabsf(near - far) < ZERO_RANGE_FLOAT) {
		return NULL;
	}

    t = fabsf(near) * tan(0.5f * fov);
    b = -t;
    r = aspect * t;
    l = -r;

	mat4x4f_unit(&tmp);

	tmp.m00 = 2.0f * near / (r - l);
    tmp.m02 = (l + r) / (l - r);
	tmp.m11 = 2.0f * near / (t - b);
	tmp.m12 = (b + t) / (b - t);
	tmp.m22 = (far + near) / (near - far);
	tmp.m23 = 2.0f * far * near / (far - near);
	tmp.m32 = 1.0f;
    tmp.m33 = 0.0f;

	mat4x4f_mult(&tmp, mat, mat);
	return mat;
}
#endif

struct mat4x4f *mat4x4f_lookat(
	struct mat4x4f *mat, const struct vec4f *pos, const struct vec4f *target, const struct vec4f *up) 
{
	struct vec4f u;
	struct vec4f v;
	struct vec4f n;

	struct mat4x4f tmp;

	if (!mat || !pos || !target || !up) {
		return NULL;
	}

	vec4f_zero(&u);
	vec4f_zero(&v);
	vec4f_zero(&n);
	mat4x4f_unit(&tmp);

	/* 摄像机坐标系为右手笛卡尔坐标系 -n 方向为视线方向 */

	/* 相机位置向量 - 目标位置向量 = 目标平面法线 n */
	vec4f_sub(pos, target, &n);
	vec4f_normalize(&n);

	/* 相机坐标系 up X n(也就是相机坐标系 -z 方向) = 相机坐标系 x 方向向量 u */
	vec4f_cross(up, &n, &u);
	vec4f_normalize(&u);

	/* 反求相机 y 轴向量 v */
	vec4f_cross(&n, &u, &v);

	/* 设置矩阵 */
	tmp.m00 = u.x;
	tmp.m01 = u.y;
	tmp.m02 = u.z;
	tmp.m10 = v.x;
	tmp.m11 = v.y;
	tmp.m12 = v.z;
	tmp.m20 = n.x;
	tmp.m21 = n.y;
	tmp.m22 = n.z;
	tmp.m03 = -vec4f_dot(&u, pos);
	tmp.m13 = -vec4f_dot(&v, pos);
	tmp.m23 = -vec4f_dot(&n, pos);

	mat4x4f_mult(&tmp, mat, mat);
	return mat;
}

/*===========================================================================*/
