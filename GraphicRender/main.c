#include <stddef.h>
#include "mem/gr_mem.h"
#include "math/gr_vec2f.h"

#include "gr_engine.h"

/*---------------------------------------------------------------------------*/

GR_UINT32 render_update(GR_ENGINE *engine, void *args)
{
    GR_VEC2F pt = { 0.0f, 0.0f };

    gr_engine_draw_line(engine, 0, 0, 479, 359, 0xFFFFFFFF);
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
