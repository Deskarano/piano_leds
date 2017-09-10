#ifndef PIANO_LEDS_COLOR_UTILS_H
#define PIANO_LEDS_COLOR_UTILS_H

unsigned int adjacent_color(unsigned int color, double factor);
unsigned int random_near_color(unsigned int color, char dr_max, char db_max, char dg_max);

#endif //PIANO_LEDS_COLOR_UTILS_H
