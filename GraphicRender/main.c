#include <stddef.h>
#include "gr_engine.h"
#include "mem/gr_mem.h"

/*---------------------------------------------------------------------------*/

GR_UINT32 render_update(GR_ENGINE *engine, void *args)
{
    gr_engine_draw_pixel(engine, 100, 100, 0x00FF00FF);
    gr_engine_draw_line(engine, 105, 105, 300, 200, 0x00FF00FF);
    return 0;
}

/*---------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    GR_ENGINE *engine = NULL;
    GR_MEM_START;

    engine = gr_engine_create(480, 360);
    gr_engine_set_update(engine, render_update, NULL);
    gr_engine_render(engine);
    gr_engine_destroy(engine);

    GR_PRINT_LEAK_INFO;
    GR_MEM_END;
    return 0;
}

/*---------------------------------------------------------------------------*/
