#ifndef SMART_LEDS_LED_PATTERNS_COLOR_UTILS_H
#define SMART_LEDS_LED_PATTERNS_COLOR_UTILS_H

/**
 * Normalizes a color so that the maximum color channel is equal to max_intensity
 * @param color The color to change
 * @param max_intensity The maximum intensity of any of the color channels
 * @return The modified color
 */
unsigned int normalize_color(unsigned int color, unsigned char max_intensity);

/**
 * Returns a color brighter by a degree of factor
 * @param color The color to change
 * @param factor The factor to change each channel by
 * @return The modified color
 */
unsigned int adjacent_color(unsigned int color, double factor);

/**
 * Returns a random color that is no further than dr_max, db_max, dg_max away from the supplied one
 *
 * @param color The color to change
 * @param dr_max The maximum change to the red channel
 * @param db_max The maximum change to the blue channel
 * @param dg_max The maximum change to the green channel
 * @return The modified color
 */
unsigned int random_near_color(unsigned int color, char dr_max, char db_max, char dg_max);

#endif //SMART_LEDS_LED_PATTERNS_COLOR_UTILS_H