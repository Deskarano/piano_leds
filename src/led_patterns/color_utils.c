#include "color_utils.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

unsigned char extract_red(unsigned int color)
{
    return (unsigned char) (color & 0xFF);
}

unsigned char extract_blue(unsigned int color)
{
    return (unsigned char) ((color >> 8) & 0xFF);
}

unsigned char extract_green(unsigned int color)
{
    return (unsigned char) ((color >> 16) & 0xFF);
}

unsigned int color_from_channels(unsigned char r, unsigned char b, unsigned char g)
{
    return (((unsigned int) g) << 16) + (((unsigned int) b) << 8) + (unsigned int) r;
}

unsigned int adjacent_color(unsigned int color, double factor)
{
    double r = extract_red(color) * factor;
    double b = extract_blue(color) * factor;
    double g = extract_green(color) * factor;

    return color_from_channels((unsigned char) r, (unsigned char) g, (unsigned char) b);
}

unsigned int normalize_color(unsigned int color, char max_intensity)
{
    double r = extract_red(color);
    double b = extract_blue(color);
    double g = extract_green(color);

    double max = fmax(r, fmax(b, g));

    r *= max_intensity / max;
    b *= max_intensity / max;
    g *= max_intensity / max;

    return color_from_channels((unsigned char) r, (unsigned char) g, (unsigned char) b);
}

unsigned int random_near_color(unsigned int color, char dr_max, char db_max, char dg_max)
{
    unsigned char r = extract_red(color);
    unsigned char b = extract_blue(color);
    unsigned char g = extract_green(color);

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

    return color_from_channels(r, g, b);
}