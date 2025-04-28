#ifndef __GR_WINDOW_H__
#define __GR_WINDOW_H__

#include "gr_type.h"
#include "gr_event.h"

/*---------------------------------------------------------------------------*/
/* 窗口对象(内) */
/*---------------------------------------------------------------------------*/

/* 窗口对象 */
typedef struct gr_window GR_WINDOW;

/* 创建窗口 */
GR_WINDOW *gr_create_window(int width, int height);

/* 销毁窗口*/
void gr_destroy_window(GR_WINDOW *window);

/* 窗口事件捕捉 */
GR_BOOL gr_win_event_capture(GR_WINDOW *window, GR_EVENT *evt);

/* 窗口渲染，bw 和 bh 表示颜色缓冲的宽度和高度 */
void gr_win_render(GR_WINDOW *window, const GR_UINT32 *buffer, int bw, int bh);

/* 等待 */
void gr_win_delay_ms(GR_UINT32 ms);

/*---------------------------------------------------------------------------*/

#endif /* __GR_WINDOW_H__ */