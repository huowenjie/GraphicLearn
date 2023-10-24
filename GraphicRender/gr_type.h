#ifndef __GR_TYPE_H__
#define __GR_TYPE_H__

/*---------------------------------------------------------------------------*/
/* 基础类型定义 */
/*---------------------------------------------------------------------------*/

/* 基本数据类型 */
typedef char            GR_INT8;
typedef unsigned char   GR_UINT8;
typedef int             GR_INT32;
typedef unsigned int    GR_UINT32;
typedef int             GR_BOOL;

#define GR_TRUE 1
#define GR_FALSE 0

/* 引擎对象 */
typedef struct gr_engine GR_ENGINE;

/* 窗口对象 */
typedef struct gr_window GR_WINDOW;

/* 主循环更新回调 */
typedef GR_UINT32 (*GR_LOOP_UPDATE)(GR_ENGINE *, void *);

/*---------------------------------------------------------------------------*/

#endif /* __GR_TYPE_H__ */
