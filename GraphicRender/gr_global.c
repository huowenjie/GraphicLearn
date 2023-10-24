#include "gr_global.h"

/*===========================================================================*/

float gr_clampf(float val, float max, float min)
{
    if (val > max) {
        return max;
    } else if (val < min) {
        return min;
    }

    return val;
}

float gr_maxf(float a, float b)
{
    return a > b ? a : b;
}

float gr_minf(float a, float b)
{
    return a < b ? a : b;
}

/*===========================================================================*/
