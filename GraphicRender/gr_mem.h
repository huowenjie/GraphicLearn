#ifndef __GR_MEM_H__
#define __GR_MEM_H__

#include <stddef.h>
#include <stdlib.h>

/*===========================================================================*/
/* 内存追踪 */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 内存追踪通用宏 */
#ifdef GR_MEMORY_DEBUG
    #define GR_MEM_START create_res()
    #define GR_MEM_END clear_res()

    #define GR_MALLOC(len) mem_dbg_malloc((len), __FUNCTION__, __FILE__, __LINE__)
    #define GR_REALLOC(p, len) mem_dbg_realloc((p), (len), __FUNCTION__, __FILE__, __LINE__)
    #define GR_FREE(p) mem_dbg_free(p)

    #define GR_PRINT_MEM_INFO mem_dbg_print_info(0)
    #define GR_PRINT_BLOCK_LIST(len) mem_dbg_print_block_list(0, (len))
    #define GR_PRINT_LEAK_INFO mem_dbg_print_leak_info(0)
#else
    #define GR_MEM_START
    #define GR_MEM_END

    #define GR_MALLOC(len) malloc(len)
    #define GR_REALLOC(p, len) realloc((p), (len))
    #define GR_FREE(p) free(p)

    #define GR_PRINT_MEM_INFO
    #define GR_PRINT_BLOCK_LIST
    #define GR_PRINT_LEAK_INFO
#endif /* USE_MEMORY */

#define GR_MEM_CLEAR(p, len) mem_clear((p), (len))
#define GR_MEM_CLEAR_FREE(p, len) (GR_MEM_CLEAR((p), (len)), GR_FREE((p)))

/* 初始化内存资源 */
void create_res();

/* 销毁内存资源 */
void clear_res();

/* 通用内存管理函数 */
void *mem_malloc(size_t len);
void *mem_realloc(void *ptr, size_t len);
void  mem_free(void *ptr);

/* 携带调试的内存管理函数 */
void *mem_dbg_malloc(size_t len, const char *func, const char *file, int line);
void *mem_dbg_realloc(void *ptr, size_t len, const char *func, const char *file, int line);
void  mem_dbg_free(void *ptr);

/* 内存擦除 */
void mem_clear(void *ptr, size_t len);

/* 打印内存信息 */
void mem_print_info();
void mem_dbg_print_info();

/* 打印内存块信息 */
void mem_print_block_list(size_t len);
void mem_dbg_print_block_list(size_t len);

/* 打印泄漏信息 */
void mem_print_leak_info();
void mem_dbg_print_leak_info();

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================*/

#endif /* __GR_MEM_H__ */
