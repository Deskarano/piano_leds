#include "color_utils.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

unsigned int adjacent_color(unsigned int color, double factor)
{
    double r = (double) (color & 0xFF) * factor;
    double b = (double) ((color >> 8) & 0xFF) * factor;
    double g = (double) ((color >> 16) & 0xFF) * factor;

    unsigned int ret = (((unsigned int) g) << 16) + (((unsigned int) b) << 8) + (unsigned int) r;
    return ret;
}

unsigned int maximize_color(unsigned int color)
{
    double r = (double) (color & 0xFF);
    double b = (double) ((color >> 8) & 0xFF);
    double g = (double) ((color >> 16) & 0xFF);

    double max = fmax(r, fmax(b, g));

    r *= 255 / max;
    b *= 255 / max;
    g *= 255 / max;

    unsigned int ret = (((unsigned int) g) << 16) + (((unsigned int) b) << 8) + (unsigned int) r;
    return ret;
}

unsigned int random_near_color(unsigned int color, char dr_max, char db_max, char dg_max)
{
    char r = (char) (color & 0xFF);
    char b = (char) ((color >> 8) & 0xFF);
    char g = (char) ((color >> 16) & 0xFF);

    srandom((unsigned int) time(NULL));

    char dr = (char) (random() % (2 * dr_max));
    if(dr > dr_max && r < (0xFF - (dr - dr_max)))
    {
        r += (dr - dr_max);
    }
    else if(dr < dr_max && r > dr)
    {
        r -= dr;
    }

    char db = (char) (random() % (2 * db_max));
    if(db > db_max && b < (0xFF - (db - db_max)))
    {
        b += (db - db_max);
    }
    else if(db < db_max && b > db)
    {
        b -= db;
    }

    char dg = (char) (random() % (2 * dg_max));
    if(dg > dg_max && g < (0xFF - (dg - dg_max)))
    {
        g += (dg - dg_max);
    }
    else if(dg < dg_max && g > dg)
    {
        g -= dg;
    }

    unsigned int ret = (((unsigned int) g) << 16) + (((unsigned int) b) << 8) + (unsigned int) r;
    return ret;
}