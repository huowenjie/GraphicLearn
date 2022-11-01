#include "sr_global.h"

//-----------------------------------------------------------------------------

float srtClampf(float val, float max, float min)
{
    if (val > max) {
        return max;
    } else if (val < min) {
        return min;
    }

    return val;
}

float srtMaxf(float a, float b)
{
    return a > b ? a : b;
}

float srtMinf(float a, float b)
{
    return a < b ? a : b;
}

//-----------------------------------------------------------------------------
