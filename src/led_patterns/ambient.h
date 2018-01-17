#ifndef SMART_LEDS_LED_PATTERNS_AMBIENT_H
#define SMART_LEDS_LED_PATTERNS_AMBIENT_H

#include "core.h"

/**
 * Function to create and initialize a new led_update_ambient_normal_data_t
 * @return A pointer to the new led_update_ambient_normal_data_t
 */
void *new_led_update_ambient_normal_data_t();

/**
 * Frees all memory used by a given led_update_ambient_normal_data_t
 * @param data The led_update_ambient_normal_data_t to free
 */
void free_led_update_ambient_normal_data_t(void *data);

/**
 * Function to create and initialize a new led_update_ambient_gradient_data_t
 * @return A pointer to the new led_update_ambient_gradient_data_t
 */
void *new_led_update_ambient_gradient_data_t();

/**
 * Frees all memory used by a given led_update_ambient_gradient_data_t
 * @param data The led_update_ambient_gradient_data_t to free
 */
void free_led_update_ambient_gradient_data_t(void *data);

/**
 * The normal ambient LED function. Gently changes color of the entire strip over time
 * @param data The filled out data struct for led_update_ambient_normal
 */
void led_update_ambient_normal(led_update_function_data_t *data);

/**
 * The gradient ambient LED function. Changes the gradient color between the left and right ends of the strip over time
 * @param data The filled out data struct for led_update_ambient_gradient
 */
void led_update_ambient_gradient(led_update_function_data_t *data);

#endif //SMART_LEDS_LED_PATTERNS_AMBIENT_H