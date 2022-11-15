#include "sr_global.h"

//-----------------------------------------------------------------------------

float srClampf(float val, float max, float min)
{
    if (val > max) {
        return max;
    } else if (val < min) {
        return min;
    }

    return val;
}

float srMaxf(float a, float b)
{
    return a > b ? a : b;
}

float srMinf(float a, float b)
{
    return a < b ? a : b;
}

//-----------------------------------------------------------------------------
