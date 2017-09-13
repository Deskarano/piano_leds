#ifndef PIANO_LEDS_LED_PATTERNS_H
#define PIANO_LEDS_LED_PATTERNS_H

#include "../pipe/pipe.h"
#include "../globals.h"

typedef void (*led_update_function)(struct led_update_function_data *);

typedef struct led_update_function_data
{
    led_update_function update_function;
    led_update_function prev_update_function;
    
    void *pattern_data;

    unsigned int led_states[LED_COUNT];
    unsigned char buffer[2];

    pipe_consumer_t *consumer;
} led_update_function_data_t;

led_update_function_data_t *new_led_update_function_data_t();

void led_update_piano_normal(led_update_function_data_t *);
void led_update_piano_war(led_update_function_data_t *);

void run_led_update_function(led_update_function_data_t *);

#endif //PIANO_LEDS_LED_PATTERNS_H
