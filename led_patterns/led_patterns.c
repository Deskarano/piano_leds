#include "led_patterns.h"
#include "color_utils.h"

#include <stdlib.h>

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

    ret->last_color = 0;
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
        ret->locked[i] = 0;
    }

    return ret;
}

void led_update_piano_normal(ws2811_t *led_string, pipe_consumer_t *consumer, led_update_function_data *data)
{
    if(pipe_size((pipe_generic_t *) consumer) > 0)
    {
        ws2811_led_t color = data->piano_normal->last_color;

        if(color == 0)
        {
            color = (unsigned int) (random() & 0xFFFFFF);
        }
        else
        {
            color = random_near_color(data->piano_normal->last_color, 8, 8, 8);
        }
        
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

            data->piano_normal->last_color = color;
        }
    }

    if(!data->piano_normal->sustain)
    {
        for(int i = 0; i < LED_COUNT; i++)
        {
            if(data->piano_normal->key_pressed[i] == 0 && data->piano_normal->key_sustain[i] == 1)
            {
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
    //unlock all values
    for(int i = 0; i < LED_COUNT; i++)
    {
        if(data->piano_war->locked[i]) data->piano_war->locked[i] = 0;
    }

    for(int i = 0; i < LED_COUNT; i++)
    {
        if(data->piano_war->occupied[i] && !data->piano_war->locked[i])
        {
            if((i == 0 && data->piano_war->direction[i] == -1) ||
               (i == LED_COUNT - 1 && data->piano_war->direction[i] == 1))
            {
                data->piano_war->size[i]--;

                if(data->piano_war->size[i])
                {
                    data->piano_war->colors[i] = random_near_color(data->piano_war->colors[i], 32, 32, 32);

                    if(data->piano_war->direction[i] == -1)
                    {
                        data->piano_war->direction[i] = 1;
                    }
                    else if(data->piano_war->direction[i] == 1)
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
                if(data->piano_war->occupied[i + data->piano_war->direction[i]])
                {
                    data->piano_war->size[i]--;

                    if(data->piano_war->size[i])
                    {
                        data->piano_war->colors[i] = random_near_color(data->piano_war->colors[i], 32, 32, 32);

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
                    data->piano_war->locked[i + data->piano_war->direction[i]] = 1;

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

    if(left_count > 0)
    {
        data->piano_war->occupied[0] = 1;
        data->piano_war->colors[0] = (uint32_t) (random() % 0xFFFFFF);
        data->piano_war->direction[0] = 1;
        data->piano_war->size[0] = left_count;
    }

    if(right_count > 0)
    {
        data->piano_war->occupied[LED_COUNT - 1] = 1;
        data->piano_war->colors[LED_COUNT - 1] = (uint32_t) (random() % 0xFFFFFF);
        data->piano_war->direction[LED_COUNT - 1] = -1;
        data->piano_war->size[LED_COUNT - 1] = right_count;
    }

    for(int i = 0; i < LED_COUNT; i++)
    {
        led_string->channel[0].leds[i] = data->piano_war->colors[i];
    }
}
