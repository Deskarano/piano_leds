#ifndef PIANO_LEDS_MAIN_H
#define PIANO_LEDS_MAIN_H

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <math.h>

#include <alsa/asoundlib.h>

#include "rpi_ws281x/ws2811.h"
#include "pipe/pipe.h"

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                21
#define DMA                     5
#define STRIP_TYPE              WS2811_STRIP_GBR        // WS2812/SK6812RGB integrated chip+leds
#define LED_COUNT               150
#define UPDATES_PER_SEC         16

__uint32_t adjacent_color(__uint32_t color, double factor)
{
    double r = (double) (color & 0xFF) * factor;
    double b = (double) ((color >> 8) & 0xFF) * factor;
    double g = (double) ((color >> 16) & 0xFF) * factor;

    __uint32_t ret = (((__uint32_t) g) << 16) + (((__uint32_t) b) << 8) + (__uint32_t) r;

    return ret;
}

typedef struct
{
    unsigned char buffer[2];
    int key_pressed[LED_COUNT];
    int key_sustain[LED_COUNT];

    int sustain;
} led_update_react_to_piano_data;

typedef union
{
    led_update_react_to_piano_data *react_to_piano;
} led_update_function_data;

int (*led_update_function)(ws2811_t *, pipe_consumer_t *, led_update_function_data *);

led_update_react_to_piano_data *new_led_update_react_to_piano_data()
{
    led_update_react_to_piano_data *ret = malloc(sizeof(led_update_react_to_piano_data));

    ret->buffer[0] = 0;
    ret->buffer[1] = 1;

    for(int i = 0; i < LED_COUNT; i++)
    {
        ret->key_pressed[i] = 0;
        ret->key_sustain[i] = 0;
    }

    ret->sustain = 0;

    return ret;
}

int led_update_react_to_piano(ws2811_t *led_string, pipe_consumer_t *consumer, led_update_function_data *data)
{
    if(pipe_size((pipe_generic_t *) consumer) > 0)
    {
        ws2811_led_t color = (uint32_t) (random() % 0xFFFFFF);

        while(pipe_size((pipe_generic_t *) consumer) > 0)
        {
            pipe_pop(consumer, data->react_to_piano->buffer, 1);

            if(data->react_to_piano->buffer[0] == 144)
            {
                pipe_pop(consumer, data->react_to_piano->buffer, 2);

                if(data->react_to_piano->buffer[1] > 0)
                {
                    led_string->channel[0].leds[data->react_to_piano->buffer[0] - 21] = color;
                    data->react_to_piano->key_pressed[data->react_to_piano->buffer[0] - 21] = 1;
                    data->react_to_piano->key_sustain[data->react_to_piano->buffer[0] - 21] = 1;
                }
                else
                {
                    data->react_to_piano->key_pressed[data->react_to_piano->buffer[0] - 21] = 0;

                    if(!data->react_to_piano->sustain)
                    {
                        data->react_to_piano->key_sustain[data->react_to_piano->buffer[0] - 21] = 0;
                    }
                }
            }

            if(data->react_to_piano->buffer[0] == 176)
            {
                pipe_pop(consumer, data->react_to_piano->buffer, 2);

                if(data->react_to_piano->buffer[1] > 0)
                {
                    data->react_to_piano->sustain = 1;
                }
                else
                {
                    data->react_to_piano->sustain = 0;
                }
            }
        }
    }

    if(!data->react_to_piano->sustain)
    {
        for(int i = 0; i < LED_COUNT; i++)
        {
            if(data->react_to_piano->key_pressed[i] == 0 && data->react_to_piano->key_sustain[i] == 1)
            {
                printf("setting key_sustain[%i] to 0\n", i);
                data->react_to_piano->key_sustain[i] = 0;
            }
        }
    }

    for(int i = 0; i < LED_COUNT; i++)
    {
        if(!(data->react_to_piano->key_sustain[i]))
        {
            led_string->channel[0].leds[i] = adjacent_color(led_string->channel[0].leds[i], .5);
        }
        else
        {
            led_string->channel[0].leds[i] = adjacent_color(led_string->channel[0].leds[i], .95);
        }
    }
}


#endif //PIANO_LEDS_MAIN_H
