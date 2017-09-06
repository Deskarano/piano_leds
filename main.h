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
#define LED_COUNT               88
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
} led_update_piano_normal_data;

typedef struct
{
    unsigned char buffer[2];

    int occupied[LED_COUNT];
    __uint32_t colors[LED_COUNT];
    int direction[LED_COUNT];
    int size[LED_COUNT];
} led_update_piano_war_data;


led_update_piano_normal_data *new_led_update_piano_normal_data()
{
    led_update_piano_normal_data *ret = malloc(sizeof(led_update_piano_normal_data));

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

led_update_piano_war_data *new_led_update_piano_war_data()
{
    led_update_piano_war_data *ret = malloc(sizeof(led_update_piano_war_data));
    ret->buffer[0] = 0;
    ret->buffer[1] = 1;

    for(int i = 0; i < LED_COUNT; i++)
    {
        ret->occupied[i] = 0;
        ret->colors[i] = 0;
        ret->direction[i] = 0;
        ret->size[i] = 0;
    }

    return ret;
}

typedef union
{
    led_update_piano_normal_data *piano_normal;
    led_update_piano_war_data *piano_war;
} led_update_function_data;

void (*led_update_function)(ws2811_t *, pipe_consumer_t *, led_update_function_data *);

void led_update_piano_normal(ws2811_t *led_string, pipe_consumer_t *consumer, led_update_function_data *data)
{
    ws2811_led_t color = (uint32_t) (random() % 0xFFFFFF);

    while(pipe_size((pipe_generic_t *) consumer) > 0)
    {
        pipe_pop(consumer, data->piano_normal->buffer, 1);

        if(data->piano_normal->buffer[0] == 144)
        {
            pipe_pop(consumer, data->piano_normal->buffer, 2);

            if(data->piano_normal->buffer[1] > 0)
            {
                led_string->channel[0].leds[data->piano_normal->buffer[0] - 21] = color;
                data->piano_normal->key_pressed[data->piano_normal->buffer[0] - 21] = 1;
                data->piano_normal->key_sustain[data->piano_normal->buffer[0] - 21] = 1;
            }
            else
            {
                data->piano_normal->key_pressed[data->piano_normal->buffer[0] - 21] = 0;

                if(!data->piano_normal->sustain)
                {
                    data->piano_normal->key_sustain[data->piano_normal->buffer[0] - 21] = 0;
                }
            }
        }

        if(data->piano_normal->buffer[0] == 176)
        {
            pipe_pop(consumer, data->piano_normal->buffer, 2);

            if(data->piano_normal->buffer[1] > 0)
            {
                data->piano_normal->sustain = 1;
            }
            else
            {
                data->piano_normal->sustain = 0;
            }
        }
    }

    if(!data->piano_normal->sustain)
    {
        for(int i = 0; i < LED_COUNT; i++)
        {
            if(data->piano_normal->key_pressed[i] == 0 && data->piano_normal->key_sustain[i] == 1)
            {
                printf("setting key_sustain[%i] to 0\n", i);
                data->piano_normal->key_sustain[i] = 0;
            }
        }
    }

    for(int i = 0; i < LED_COUNT; i++)
    {
        if(!(data->piano_normal->key_sustain[i]))
        {
            led_string->channel[0].leds[i] = adjacent_color(led_string->channel[0].leds[i], .5);
        }
        else
        {
            led_string->channel[0].leds[i] = adjacent_color(led_string->channel[0].leds[i], .95);
        }
    }
}

void led_update_piano_war(ws2811_t *led_string, pipe_consumer_t *consumer, led_update_function_data *data)
{
    for(int i = 0; i < LED_COUNT; i++)
    {
        if(data->piano_war->occupied[i])
        {
            if(i == 0 || i == LED_COUNT - 1)
            {
                if(i == 0 && data->piano_war->direction[i] == -1)
                {
                    data->piano_war->size[i]--;

                    if(data->piano_war->size[i])
                    {
                        data->piano_war->colors[i] = (uint32_t) (random() % 0xFFFFFF);
                        data->piano_war->direction[i] = 1;
                    }
                    else
                    {
                        data->piano_war->occupied[i] = 0;
                        data->piano_war->colors[i] = 0;
                        data->piano_war->direction[i] = 0;
                    }
                }

                if(i == LED_COUNT - 1 && data->piano_war->direction[i] == 1)
                {
                    data->piano_war->size[i]--;

                    if(data->piano_war->size[i])
                    {
                        data->piano_war->colors[i] = (uint32_t) (random() % 0xFFFFFF);
                        data->piano_war->direction[i] = -1;
                    }
                    else
                    {
                        data->piano_war->occupied[i] = 0;
                        data->piano_war->colors[i] = 0;
                        data->piano_war->direction[i] = 0;
                    }
                }
            }
            else
            {
                if(data->piano_war->occupied[i + data->piano_war->direction[i]])
                {
                    data->piano_war->size[i]--;

                    if(data->piano_war->size[i])
                    {
                        data->piano_war->colors[i] = (uint32_t) (random() % 0xFFFFFF);

                        if(data->piano_war->direction[i] == -1)
                        {
                            data->piano_war->direction[i] = 1;
                        }
                        else
                        {
                            data->piano_war->direction[i] = -1;
                        }
                    }
                    else
                    {
                        data->piano_war->occupied[i] = 0;
                        data->piano_war->colors[i] = 0;
                        data->piano_war->direction[i] = 0;
                    }
                }
                else
                {
                    data->piano_war->occupied[i + data->piano_war->direction[i]] = 1;
                    data->piano_war->colors[i + data->piano_war->direction[i]] = data->piano_war->colors[i];
                    data->piano_war->direction[i + data->piano_war->direction[i]] = data->piano_war->direction[i];
                    data->piano_war->size[i + data->piano_war->direction[i]] = data->piano_war->size[i];

                    data->piano_war->occupied[i] = 0;
                    data->piano_war->colors[i] = 0;
                    data->piano_war->direction[i] = 0;
                    data->piano_war->size[i] = 0;
                }
            }
        }
    }

    int left_count = 0;
    int right_count = 0;

    while(pipe_size((pipe_generic_t *) consumer) > 0)
    {
        pipe_pop(consumer, data->piano_war->buffer, 1);

        if(data->piano_war->buffer[0] == 144)
        {
            pipe_pop(consumer, data->piano_war->buffer, 2);

            if(data->piano_war->buffer[1] > 0)
            {
                //determine which direction to send it from
                if(data->piano_war->buffer[0] - 21 < 44)
                {
                    left_count++;
                }
                else
                {
                    right_count++;
                }
            }
        }
    }

    printf("left_count: %i\nright_count: %i\n", left_count, right_count);

    if(left_count > 0)
    {
        data->piano_war->colors[0] = (uint32_t) (random() % 0xFFFFFF);
        data->piano_war->direction[0] = 1;
        data->piano_war->size[0] = left_count;
    }

    if(right_count > 0)
    {
        data->piano_war->colors[LED_COUNT - 1] = (uint32_t) (random() % 0xFFFFFF);
        data->piano_war->direction[LED_COUNT - 1] = -1;
        data->piano_war->size[LED_COUNT - 1] = right_count;
    }

    for(int i = 0; i < LED_COUNT; i++)
    {
        led_string->channel[0].leds[i] = data->piano_war->colors[i];
    }
}

#endif //PIANO_LEDS_MAIN_H