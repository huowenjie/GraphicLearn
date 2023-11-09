#include <stddef.h>
#include "mem/gr_mem.h"
#include "math/gr_vec2f.h"

#include "gr_engine.h"

/*---------------------------------------------------------------------------*/

void draw_circle(GR_ENGINE *engine, const GR_VEC2F *p, float r, GR_RGB rgb)
{
    float d = r + 10.0f;
    float i = p->x - d;
    float j = 0.0f;

    for (; i < (p->x + d); i += 1.0f) {
        for (j = p->y - d; j < (p->y + d); j += 1.0f) {
            GR_VEC2F pt = { i, j };
            GR_VEC2F v = gr_vec2f_sub(&pt, p);

            if (gr_vec2f_length(&v) <= r) {
                gr_engine_draw_pixel(engine, (int)pt.x, (int)pt.y, 0xFFFF00FF);
            }
        }
    }
}

GR_UINT32 render_update(GR_ENGINE *engine, void *args)
{
    GR_VEC2F pt = { 0.0f, 0.0f };

    draw_circle(engine, &pt, 100.0f, 0x00FF00FF);
    //gr_engine_draw_pixel(engine, (int)pt.x, (int)pt.y, 0x00FF00FF);
    gr_engine_draw_line(engine, 40, 0, 50, 359, 0xFFFF00FF);
    return 0;
}

/*---------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    GR_ENGINE *engine = NULL;
    GR_MEM_START;

    engine = gr_engine_create(480, 360);
    //engine = gr_engine_create(100, 100);
    gr_engine_set_update(engine, render_update, NULL);
    gr_engine_render(engine);
    gr_engine_destroy(engine);

    GR_PRINT_LEAK_INFO;
    GR_MEM_END;
    return 0;
}

/*---------------------------------------------------------------------------*/
