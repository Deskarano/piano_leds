#ifndef SMART_LEDS_LED_PATTERNS_COLOR_UTILS_H
#define SMART_LEDS_LED_PATTERNS_COLOR_UTILS_H

unsigned char extract_red(unsigned int color);
unsigned char extract_blue(unsigned int color);
unsigned char extract_green(unsigned int color);
unsigned int color_from_channels(unsigned char r, unsigned char b, unsigned char g);

unsigned int normalize_color(unsigned int color, char max_intensity);
unsigned int adjacent_color(unsigned int color, double factor);
unsigned int random_near_color(unsigned int color, char dr_max, char db_max, char dg_max);

#endif //SMART_LEDS_LED_PATTERNS_COLOR_UTILS_H