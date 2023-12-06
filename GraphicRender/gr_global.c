#include <stdlib.h>
#include <time.h>

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

void randinit()
{
    srand(time(0));
}

float randomf(float min, float max)
{
    int imin = (int)min;
    int imax = (int)max;
    float fval = 0.0f;

    do {
        int val = imin + rand() % (imax - imin);
        fval = (float)val;
    } while (fval < min || fval > max);

    return fval;
}

int randomi(int min, int max)
{
    int ival = 0;

    do {
        ival = min + rand() % (max - min);
    } while (ival < min || ival > max);

    return ival;
}

GR_BOOL randomb()
{
    return (rand() % 2) == 0;
}

/*===========================================================================*/
