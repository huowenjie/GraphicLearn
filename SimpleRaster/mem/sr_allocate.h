#ifndef __SR_ALLOCATE_H__
#define __SR_ALLOCATE_H__

#include <cstddef>

/*===========================================================================*/
/* C++ 内存分配控制器 */
/*===========================================================================*/

#ifdef SR_MEMORY_DEBUG
void *operator new(std::size_t size, const char *func, const char *file, int line);
void *operator new[](std::size_t size, const char *func, const char *file, int line);
void *operator new(std::size_t size);
void *operator new[](std::size_t size);
void operator delete(void *p) noexcept;
void operator delete[](void *p) noexcept;
#endif /* SR_MEMORY_DEBUG */

/*===========================================================================*/

#endif /* __SR_ALLOCATE_H__ */