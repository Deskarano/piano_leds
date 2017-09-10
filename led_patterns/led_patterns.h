#ifndef PIANO_LEDS_LED_PATTERNS_H
#define PIANO_LEDS_LED_PATTERNS_H

#include "../rpi_ws281x/ws2811.h"
#include "../pipe/pipe.h"
#include "../globals.h"

typedef struct led_update_piano_normal_data
{
    unsigned char buffer[2];
    int key_pressed[LED_COUNT];
    int key_sustain[LED_COUNT];

    unsigned int last_color;
    int sustain;
} led_update_piano_normal_data;

typedef struct led_update_piano_war_data
{
    unsigned char buffer[2];

    int occupied[LED_COUNT];
    int colors[LED_COUNT];
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

void led_update_piano_normal(ws2811_t *, pipe_consumer_t *, led_update_function_data *);
void led_update_piano_war(ws2811_t *, pipe_consumer_t *, led_update_function_data *);

#endif //PIANO_LEDS_LED_PATTERNS_H
