#ifndef __SR_MEM_H__
#define __SR_MEM_H__

#include <stddef.h>
#include <stdlib.h>

/*===========================================================================*/
/* 内存追踪 */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 内存追踪通用宏 */
#ifdef SR_MEMORY_DEBUG
    #define SR_MEM_START create_res()
    #define SR_MEM_END clear_res()

    #define SR_MALLOC(len) mem_dbg_malloc((len), __FUNCTION__, __FILE__, __LINE__)
    #define SR_REALLOC(p, len) mem_dbg_realloc((p), (len), __FUNCTION__, __FILE__, __LINE__)
    #define SR_FREE(p) mem_dbg_free(p)

    #define SR_PRINT_MEM_INFO mem_dbg_print_info()
    #define SR_PRINT_BLOCK_LIST(len) mem_dbg_print_block_list((len))
    #define SR_PRINT_LEAK_INFO mem_dbg_print_leak_info()
#else
    #define SR_MEM_START
    #define SR_MEM_END

    #define SR_MALLOC(len) malloc(len)
    #define SR_REALLOC(p, len) realloc((p), (len))
    #define SR_FREE(p) free(p)

    #define SR_PRINT_MEM_INFO
    #define SR_PRINT_BLOCK_LIST
    #define SR_PRINT_LEAK_INFO
#endif /* USE_MEMORY */

#define SR_MEM_CLEAR(p, len) mem_clear((p), (len))
#define SR_MEM_CLEAR_FREE(p, len) (SR_MEM_CLEAR((p), (len)), SR_FREE((p)))

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

#endif /* __SR_MEM_H__ */
