#include "srt_global.h"

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

//-----------------------------------------------------------------------------
