#ifndef __GR_ENGINE_H__
#define __GR_ENGINE_H__

#include "math/gr_vec2f.h"
#include "gr_color.h"

/*===========================================================================*/
/* 引擎对象 */
/*===========================================================================*/

/* 创建引擎 */
GR_ENGINE *gr_engine_create(int width, int height);

/* 销毁引擎 */
void gr_engine_destroy(GR_ENGINE *engine);

/* 设置更新回调 */
void gr_engine_set_update(GR_ENGINE *engine, GR_LOOP_UPDATE callback, void *args);

/* 启动主循环渲染 */
void gr_engine_render(GR_ENGINE *engine);

/* 绘制像素点 */
void gr_engine_draw_pixel(GR_ENGINE *engine, int x, int y, GR_RGB rgb);

/* 绘制线 */
void gr_engine_draw_line(GR_ENGINE *engine, int ax, int ay, int bx, int by, GR_RGB rgb);

/*===========================================================================*/

#endif /* __GR_ENGINE_H__ */