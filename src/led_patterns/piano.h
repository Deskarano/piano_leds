#ifndef SMART_LEDS_LED_PATTERNS_PIANO_H
#define SMART_LEDS_LED_PATTERNS_PIANO_H

#include "core.h"

/**
 * Function to create and initialize a new led_update_piano_normal_data_t
 * @return A pointer to the new led_update_piano_normal_data_t
 */
void *new_led_update_piano_normal_data_t();

/**
 * Frees all memory used by a given led_update_piano_normal_data_t
 * @param data The led_update_piano_normal_data_t to free
 */
void free_led_update_piano_normal_data_t(void *data);

/**
 * Function to create and initialize a new led_update_piano_war_data_t
 *
 * @return A pointer to the new led_update_piano_war_data_t
 */
void *new_led_update_piano_war_data_t();

/**
 * Frees all memory used by a given led_update_piano_war_data_t
 * @param data The led_update_piano_war_data_t to free
 */
void free_led_update_piano_war_data_t(void *data);

/**
 * The normal piano-enabled LED update function. Each key lights up one specific LED
 * @param data The filled out data struct for led_update_piano_normal
 */
void led_update_piano_normal(led_update_function_data_t *data);

/**
 * The war piano-enabled LED update. Each key sends a burst of light from one end
 * @param data The filled out data struct for led_update_piano_war
 */
void led_update_piano_war(led_update_function_data_t *data);

#endif //SMART_LEDS_LED_PATTERNS_H