#ifndef __GR_EVENT_H__
#define __GR_EVENT_H__

#include "gr_type.h"

/*===========================================================================*/
/* 事件定义 */
/*===========================================================================*/

/* 事件类型 */
typedef enum gr_event_type
{
    /* 未定义事件 */
    GR_EVT_UNDEF = 0x00000000,

    /* 窗口事件 */
    GR_EVT_WIN = 0x00000001
} GR_EVENT_TYPE;

/* 窗口事件 */
typedef enum gr_event_window
{
    /* 窗口关闭事件 */
    GR_EVT_WIN_CLOSE = 0x00000000
} GR_EVENT_WINDOW;

/* 事件定义 */
typedef struct gr_event
{
    GR_EVENT_TYPE type;

    /* 具体事件代码或值 */
    union {
        GR_EVENT_WINDOW winevt;
    };
} GR_EVENT;

/*===========================================================================*/

#endif /* __GR_EVENT_H__ */
