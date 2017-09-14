#ifndef SMART_LEDS_LED_PATTERNS_COLOR_UTILS_H
#define SMART_LEDS_LED_PATTERNS_COLOR_UTILS_H

unsigned int normalize_color(unsigned int color, char max_intensity);
unsigned int adjacent_color(unsigned int color, double factor);
unsigned int random_near_color(unsigned int color, char dr_max, char db_max, char dg_max);

#endif //SMART_LEDS_LED_PATTERNS_COLOR_UTILS_H