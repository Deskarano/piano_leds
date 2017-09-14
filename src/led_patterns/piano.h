#ifndef SMART_LEDS_LED_PATTERNS_PIANO_H
#define SMART_LEDS_LED_PATTERNS_PIANO_H

#include "core.h"

/**
 * Function to create and initialize a new led_update_piano_normal_data_t
 * @return A pointer to the new led_update_piano_normal_data_t
 */
void *new_led_update_piano_normal_data_t();

/**
 * Function to create and initialize a new led_update_piano_war_data_t
 * @return A pointer to the new led_update_piano_war_data_t
 */
void *new_led_update_piano_war_data_t();

/**
 * The normal piano-enabled LED update function. Each key lights up one specific LED
 */
void led_update_piano_normal(led_update_function_data_t *);

/**
 * The war piano-enabled LED update. Each key sends a burst of light from one end
 */
void led_update_piano_war(led_update_function_data_t *);

#endif //SMART_LEDS_LED_PATTERNS_H