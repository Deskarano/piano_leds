#ifndef PIANO_LEDS_LED_PATTERNS_CORE_H
#define PIANO_LEDS_LED_PATTERNS_CORE_H

#include "../globals.h"

typedef void (*led_update_function)(struct led_update_function_data *);

typedef enum PATTERN
{
    NONE,
    PIANO_NORMAL,
    PIANO_WAR
} PATTERN;

typedef struct led_update_function_data
{
    PATTERN current_pattern;
    PATTERN last_pattern;

    void *pattern_data;

    unsigned int led_states[LED_COUNT];
    unsigned char buffer[2];

    void *consumer;
} led_update_function_data_t;

led_update_function_data_t *new_led_update_function_data_t();

void run_led_update_function(led_update_function_data_t *);

#endif //PIANO_LEDS_LED_PATTERNS_CORE_H
