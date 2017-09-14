#ifndef PIANO_LEDS_LED_PATTERNS_H
#define PIANO_LEDS_LED_PATTERNS_H

#include "led_patterns_core.h"

void *new_led_update_piano_normal_data();
void *new_led_update_piano_war_data();

void led_update_piano_normal(led_update_function_data_t *);
void led_update_piano_war(led_update_function_data_t *);

#endif //PIANO_LEDS_LED_PATTERNS_H