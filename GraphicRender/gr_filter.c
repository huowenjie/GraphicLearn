#include <stddef.h>

#include "gr_mem.h"
#include "gr_filter.h"

/*===========================================================================*/

struct gr_filter
{
    GR_FILTER_TYPE type;
    int radius;

    float (*get_value)(const GR_FILTER *filter, int i, int j);
};

/*===========================================================================*/

/* 盒型滤波器 */
GR_FILTER *create_box_filter();
float box_filter_get_value(const GR_FILTER *filter, int i, int j);

/*===========================================================================*/

GR_FILTER *gr_filter_create(GR_FILTER_TYPE type)
{
    GR_FILTER *filter = NULL;

    switch (type) {
        case GR_FT_BOX_FILTER:
            filter = create_box_filter();
            break;

        default:
            return NULL;
    }

    return filter;
}

void gr_filter_destroy(GR_FILTER *filter)
{
    if (filter) {
        GR_FREE(filter);
    }
}

float gr_filter_get(const GR_FILTER *filter, int i, int j)
{
    if (!filter || !filter->get_value) {
        return 0.0f;
    }
    return filter->get_value(filter, i, j);
}

int gr_filter_radius(const GR_FILTER *filter)
{
    if (filter) {
        return filter->radius;
    }
    return 0;
}

/*===========================================================================*/

GR_FILTER *create_box_filter()
{
    GR_FILTER *filter = GR_MALLOC(sizeof(GR_FILTER));

    if (!filter) {
        return NULL;
    }

    filter->type = GR_FT_BOX_FILTER;
    filter->radius = 1;
    filter->get_value = box_filter_get_value;
    return filter;
}

float box_filter_get_value(const GR_FILTER *filter, int i, int j)
{
    int ir = 0;
    float fr = 0.0f;
    float val = 0.0f;

    if (!filter) {
        return 0.0f;
    }

    ir = filter->radius;
    fr = (float)ir;

    if (i >= -ir && i <= ir) {
        if (j >= -ir && j <= ir) {
            val = 1.0f / ((fr + fr + 1.0f) * (fr + fr + 1.0f));
        }
    }

    return val;
}

/*===========================================================================*/
