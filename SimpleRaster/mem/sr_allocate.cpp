#include "sr_allocate.h"
#include "sr_mem.h"

/*===========================================================================*/

#ifdef SR_MEMORY_DEBUG
void *operator new(std::size_t size, const char *func, const char *file, int line)
{
    return mem_dbg_malloc(size, func, file, line);
}

void *operator new[](std::size_t size, const char *func, const char *file, int line)
{
    return mem_dbg_malloc(size, func, file, line);
}

void *operator new(std::size_t size)
{
    return mem_dbg_malloc(size, __FUNCTION__, __FILE__, __LINE__);
}

void *operator new[](std::size_t size)
{
    return mem_dbg_malloc(size, __FUNCTION__, __FILE__, __LINE__);
}

void operator delete(void *p) noexcept
{
    mem_dbg_free(p);
}

void operator delete[](void *p) noexcept
{
    mem_dbg_free(p);
}
#endif /* SR_MEMORY_DEBUG */

/*===========================================================================*/
