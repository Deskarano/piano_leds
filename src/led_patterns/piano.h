#ifndef SMART_LEDS_LED_PATTERNS_PIANO_H
#define SMART_LEDS_LED_PATTERNS_PIANO_H

#include "core.h"

void *new_led_update_piano_normal_data_t();
void *new_led_update_piano_war_data_t();

void led_update_piano_normal(led_update_function_data_t *);
void led_update_piano_war(led_update_function_data_t *);

#endif //SMART_LEDS_LED_PATTERNS_H