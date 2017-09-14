#ifndef PIANO_LEDS_UTILS_H
#define PIANO_LEDS_UTILS_H

#include "core.h"

/**
 * Function to set up an led_update_function_data_t to work with a specified LED pattern
 *
 * @param data The data struct to set all the values in
 * @param pattern The pattern to initiate in the data struct
 */
void set_data_for_pattern(led_update_function_data_t *data, Pattern pattern);

#endif //PIANO_LEDS_UTILS_H
