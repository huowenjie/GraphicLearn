#ifndef __GR_FILTER_H__
#define __GR_FILTER_H__

/*===========================================================================*/
/* 滤波器 */
/*===========================================================================*/

/* 滤波器 */
typedef struct gr_filter GR_FILTER;

/* 滤波器类型 */
typedef enum 
{
    /* 盒型滤波器 */
    GR_FT_BOX_FILTER = 0
} GR_FILTER_TYPE;

/* 创建滤波器 */
GR_FILTER *gr_filter_create(GR_FILTER_TYPE type);

/* 销毁滤波器 */
void gr_filter_destroy(GR_FILTER *filter);

/* 获取滤波器值，index 范围为 [0, 2r] 2r + 1 个值 */
float gr_filter_get(const GR_FILTER *filter, int i, int j);

/* 获取过滤半径 */
int gr_filter_radius(const GR_FILTER *filter);

/*===========================================================================*/

#endif /* __GR_FILTER_H__ */
