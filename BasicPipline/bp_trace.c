#include <stdio.h>
#include "bp_trace.h"

/*===========================================================================*/

void bp_print_vector(const char *desc, const struct vec4f *vec)
{
    if (vec) {
        printf("%s -- x:%f, y:%f, z:%f, w:%f\n", desc, vec->x, vec->y, vec->z, vec->w);
    }
}

void bp_print_matrix(const char *desc, const struct mat4x4f *mat)
{
    if (mat) {
        printf(
            "matrix:%s\n"
            "[%8.2f %8.2f %8.2f %8.2f]\n"
            "[%8.2f %8.2f %8.2f %8.2f]\n"
            "[%8.2f %8.2f %8.2f %8.2f]\n"
            "[%8.2f %8.2f %8.2f %8.2f]\n",
            desc,
            mat->m00, mat->m01, mat->m02, mat->m03,
            mat->m10, mat->m11, mat->m12, mat->m13,
            mat->m20, mat->m21, mat->m22, mat->m23,
            mat->m30, mat->m31, mat->m32, mat->m33);
    }
}

/*===========================================================================*/
