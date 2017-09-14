#ifndef SMART_LEDS_LED_PATTERNS_CORE_H
#define SMART_LEDS_LED_PATTERNS_CORE_H

#include "../globals.h"

typedef enum PATTERN
{
    NONE,
    PIANO_NORMAL,
    PIANO_WAR,
    AMBIENT_NORMAL
} Pattern;

struct led_update_function_data;

typedef void (*led_pattern_function)(struct led_update_function_data *);

typedef struct led_update_function_data
{
    Pattern current_pattern;
    Pattern last_pattern;

    led_pattern_function current_update_function;

    void *pattern_data;

    unsigned int led_states[LED_COUNT];
    unsigned char buffer[2];

    void *data_pipe;
} led_update_function_data_t;

led_update_function_data_t *new_led_update_function_data_t();

void run_led_update_function(led_update_function_data_t *);

#endif //SMART_LEDS_LED_PATTERNS_CORE_H