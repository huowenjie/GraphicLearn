#ifndef __GR_GLOBAL_H__
#define __GR_GLOBAL_H__

#include "gr_type.h"

/*===========================================================================*/
/* 全局定义 */
/*===========================================================================*/

/* 圆周率 */

#define GR_PI    3.1415926536f
#define GR_DB_PI 6.2831853072f

/* 夹逼 */
float gr_clampf(float val, float max, float min);

/* 选择最值*/
float gr_maxf(float a, float b);
float gr_minf(float a, float b);

/* 随机数初始化 */
void randinit();

/* 随机浮点数 */
float randomf(float min, float max);

/* 随机整数 */
int randomi(int min, int max);

/* 随机布尔值 */
GR_BOOL randomb();

/*===========================================================================*/

#endif /* __GR_GLOBAL_H__ */
