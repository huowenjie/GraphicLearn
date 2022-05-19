#include <math.h>
#include <stddef.h>
#include "bp_vector.h"

/*===========================================================================*/

#define ZERO_RANGE_FLOAT (1e-6f)

struct vec4f *vec4f_zero(struct vec4f *v)
{
	if (!v) {
		return NULL;
	}

	v->x = 0.0f;
	v->y = 0.0f;
	v->z = 0.0f;
	v->w = 0.0f;
	return v;
}

struct vec4f *vec4f_zero_pt(struct vec4f *v) 
{
	if (!v) {
		return NULL;
	}

	v->x = 0.0f;
	v->y = 0.0f;
	v->z = 0.0f;
	v->w = 1.0f;
	return v;
}

struct vec4f *vec4f_opposite(const struct vec4f *v, struct vec4f *ret)
{
	if (v) {
		struct vec4f tmp;

		tmp.x = -v->x;
		tmp.y = -v->y;
		tmp.z = -v->z;
		tmp.w =  v->w;

		*ret = tmp;
		return ret;
	}
	return NULL;
}

float vec4f_dot(const struct vec4f *v1, const struct vec4f *v2) 
{
	float ret = 0.0f;

	if (!v1 || !v2) {
		return ret;
	}

	ret = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
	return ret;
}

struct vec4f * vec4f_cross(const struct vec4f *v1, const struct vec4f *v2, struct vec4f *ret) 
{
	if (!v1 || !v2 || !ret) {
		return NULL;
	}

	ret->x = v1->y * v2->z - v1->z * v2->y;
	ret->y = v1->z * v2->x - v1->x * v2->z;
	ret->z = v1->x * v2->y - v1->y * v2->x;

	return ret;
}

struct vec4f *vec4f_normalize(struct vec4f *v)
{
	float norm = 0.0f;
	struct vec4f tmp;

	if (!v) {
		return NULL;
	}

	vec4f_zero(&tmp);

	tmp.x = v->x;
	tmp.y = v->y;
	tmp.z = v->z;

	norm = sqrtf(
		tmp.x * tmp.x +
		tmp.y * tmp.y +
		tmp.z * tmp.z);

	if (norm < ZERO_RANGE_FLOAT) {
		return NULL;
	}

	tmp.x /= norm;
	tmp.y /= norm;
	tmp.z /= norm;
	tmp.w = 0.0f;
	*v = tmp;

	return v;
}

struct vec4f *vec4f_homogeneous(struct vec4f *v)
{
	float w = 0.0f;
	struct vec4f tmp;

	if (!v || fabsf(v->w) < ZERO_RANGE_FLOAT) {
		return NULL;
	}

	vec4f_zero(&tmp);

	tmp.x = v->x;
	tmp.y = v->y;
	tmp.z = v->z;
    w = v->w;

	tmp.x /= w;
	tmp.y /= w;
	tmp.z /= w;
	tmp.w = 1.0f;
	*v = tmp;

	return v;
}

struct vec4f * vec4f_add(const struct vec4f *v1, const struct vec4f *v2, struct vec4f *ret)
{
	if (!v1 || !v2 || !ret) {
		return NULL;
	}

	ret->x = v1->x + v2->x;
	ret->y = v1->y + v2->y;
	ret->z = v1->z + v2->z;

	return ret;
}

struct vec4f * vec4f_sub(const struct vec4f *v1, const struct vec4f *v2, struct vec4f *ret)
{
	if (!v1 || !v2 || !ret) {
		return NULL;
	}

	ret->x = v1->x - v2->x;
	ret->y = v1->y - v2->y;
	ret->z = v1->z - v2->z;

	return ret;
}

struct vec4f * vec4f_mult(const struct vec4f *v, float n, struct vec4f *ret) 
{
	if (!v || !ret) {
		return NULL;
	}

	ret->x = v->x * n;
	ret->y = v->y * n;
	ret->z = v->z * n;

	return ret;
}

struct vec4f *vec4f_normal(const struct vec4f *v1, const struct vec4f *v2, struct vec4f *normal) 
{
    struct vec4f n;

    if (!v1 || v2 || !normal) {
        return NULL;
    }

	vec4f_zero(&n);

	vec4f_cross(v1, v2, &n);
	vec4f_normalize(&n);

	*normal = n;
	return normal;
}

/*===========================================================================*/
