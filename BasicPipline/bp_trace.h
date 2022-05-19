#ifndef BP_TRACE_H
#define BP_TRACE_H

#include "bp_matrix.h"

/*===========================================================================*/
/* 追踪 */
/*===========================================================================*/

/* 打印点或向量 */
void bp_print_vector(const char *desc, const struct vec4f *vec);

/* 打印矩阵 */
void bp_print_matrix(const char *desc, const struct mat4x4f *mat);

/*===========================================================================*/

#endif /* BP_TRACE_H */
