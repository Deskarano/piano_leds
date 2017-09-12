#ifndef PIANO_LEDS_LED_PATTERNS_H
#define PIANO_LEDS_LED_PATTERNS_H

#include "../pipe/pipe.h"
#include "../globals.h"

typedef struct led_update_piano_normal_data
{
    unsigned char buffer[2];
    int key_pressed[LED_COUNT];
    int key_sustain[LED_COUNT];

    unsigned int led_states[LED_COUNT];

    unsigned int last_color;
    int sustain;
} led_update_piano_normal_data;

typedef struct led_update_piano_war_data
{
    unsigned char buffer[2];

    unsigned int led_states[LED_COUNT];

    int occupied[LED_COUNT];
    unsigned int colors[LED_COUNT];
    int direction[LED_COUNT];
    int size[LED_COUNT];
    int locked[LED_COUNT];
} led_update_piano_war_data;

led_update_piano_normal_data *new_led_update_piano_normal_data();
led_update_piano_war_data *new_led_update_piano_war_data();

typedef union led_update_function_data
{
    led_update_piano_normal_data *piano_normal;
    led_update_piano_war_data *piano_war;
} led_update_function_data;

void led_update_piano_normal(pipe_consumer_t *, led_update_function_data *);
void led_update_piano_war(pipe_consumer_t *, led_update_function_data *);

void transfer_led_states(unsigned int *source, unsigned int *target);

#endif //PIANO_LEDS_LED_PATTERNS_H
