#ifndef __GR_ENGINE_H__
#define __GR_ENGINE_H__

#include "gr_vec2f.h"
#include "gr_color.h"

/*===========================================================================*/
/* 引擎对象 */
/*===========================================================================*/

/* 引擎对象 */
typedef struct gr_engine GR_ENGINE;

/* 抗锯齿方案 */
typedef enum {
    NO_AA = 0,
    MSAA = 1,
    SSAA = 2
} ANTI_ALIASING;

/* 主循环更新回调 */
typedef GR_UINT32 (*GR_LOOP_UPDATE)(GR_ENGINE *, void *);

/*===========================================================================*/

/* 创建渲染引擎对象 */
GR_ENGINE *gr_engine_create(int width, int height);

/* 创建缓冲区 */
void gr_engine_create_buffer(GR_ENGINE *engine, GR_UINT32 layer);

/* 销毁缓冲区 */
void gr_engine_destroy_buffer(GR_ENGINE *engine);

/* 销毁引擎 */
void gr_engine_destroy(GR_ENGINE *engine);

/* 设置更新回调 */
void gr_engine_set_update(GR_ENGINE *engine, GR_LOOP_UPDATE callback, void *args);

/* 设置抗锯齿方案 */
void gr_engine_set_aa(GR_ENGINE *engine, ANTI_ALIASING aa);

/* 获取宽度 */
int gr_engine_get_width(GR_ENGINE *engine);

/* 获取高度 */
int gr_engine_get_height(GR_ENGINE *engine);

/* 启动主循环渲染 */
void gr_engine_render(GR_ENGINE *engine);

/* 绘制像素点 */
void gr_engine_draw_pixel(
    GR_ENGINE *engine, const GR_VEC2F *pt, const GR_COLOR *color);

/* 绘制线 */
void gr_engine_draw_line(
    GR_ENGINE *engine, const GR_VEC2F *a, const GR_VEC2F *b, const GR_COLOR *color);

/*===========================================================================*/

#endif /* __GR_ENGINE_H__ */