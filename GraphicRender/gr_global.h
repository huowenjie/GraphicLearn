#ifndef __GR_GLOBAL_H__
#define __GR_GLOBAL_H__

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

/*===========================================================================*/

#endif /* __GR_GLOBAL_H__ */
