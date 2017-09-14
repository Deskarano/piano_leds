#include "piano.h"
#include "color_utils.h"
#include "../pipe/pipe.h"

#include <stdlib.h>

typedef struct led_update_piano_normal_data
{
    int key_pressed[LED_COUNT];
    int key_sustain[LED_COUNT];

    unsigned int last_color;
    int sustain;
} led_update_piano_normal_data_t;

typedef struct led_update_piano_war_data
{
    int occupied[LED_COUNT];
    unsigned int colors[LED_COUNT];
    int direction[LED_COUNT];
    int size[LED_COUNT];
    int locked[LED_COUNT];
} led_update_piano_war_data_t;

void *new_led_update_piano_normal_data()
{
    led_update_piano_normal_data_t *ret = malloc(sizeof(led_update_piano_normal_data_t));

    for(int i = 0; i < LED_COUNT; i++)
    {
        ret->key_pressed[i] = 0;
        ret->key_sustain[i] = 0;
    }

    ret->last_color = 0;
    ret->sustain = 0;

    return ret;
}

void *new_led_update_piano_war_data()
{
    led_update_piano_war_data_t *ret = malloc(sizeof(led_update_piano_war_data_t));

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

void led_update_piano_normal(led_update_function_data_t *data)
{
    led_update_piano_normal_data_t *pattern_data = (led_update_piano_normal_data_t *) data->pattern_data;

    if(pipe_size((pipe_generic_t *) data->data_pipe) > 0)
    {
        unsigned int color = pattern_data->last_color;

        if(color == 0)
        {
            color = (unsigned int) (random() & 0xFFFFFF);
        }
        else
        {
            color = random_near_color(pattern_data->last_color,
                                      RAND_COLOR_THRESHOLD,
                                      RAND_COLOR_THRESHOLD,
                                      RAND_COLOR_THRESHOLD);
        }

        while(pipe_size((pipe_generic_t *) data->data_pipe) > 0)
        {
            pipe_pop(data->data_pipe, data->buffer, 1);

            if(data->buffer[0] == 144)
            {
                pipe_pop(data->data_pipe, data->buffer, 2);

                if(data->buffer[1] > 0)
                {
                    data->led_states[data->buffer[0] - 21] = normalize_color(color, data->buffer[1] * (char) 2);
                    pattern_data->key_pressed[data->buffer[0] - 21] = 1;
                    pattern_data->key_sustain[data->buffer[0] - 21] = 1;
                }
                else
                {
                    pattern_data->key_pressed[data->buffer[0] - 21] = 0;

                    if(!pattern_data->sustain)
                    {
                        pattern_data->key_sustain[data->buffer[0] - 21] = 0;
                    }
                }
            }

            if(data->buffer[0] == 176)
            {
                pipe_pop(data->data_pipe, data->buffer, 2);

                if(data->buffer[1] > 0)
                {
                    pattern_data->sustain = 1;
                }
                else
                {
                    pattern_data->sustain = 0;
                }
            }

            pattern_data->last_color = color;
        }
    }

    //if not sustaining, remove any keys from sustain[] that are not key_pressed[]
    if(!pattern_data->sustain)
    {
        for(int i = 0; i < LED_COUNT; i++)
        {
            if(pattern_data->key_pressed[i] == 0 &&
               pattern_data->key_sustain[i] == 1)
            {
                pattern_data->key_sustain[i] = 0;
            }
        }
    }

    //sustain logic
    for(int i = 0; i < LED_COUNT; i++)
    {
        if(!(pattern_data->key_sustain[i]))
        {
            data->led_states[i] = adjacent_color(data->led_states[i], DECAY_IF_N_SUSTAIN);
        }
        else
        {
            data->led_states[i] = adjacent_color(data->led_states[i], DECAY_IF_SUSTAIN);
        }
    }
}

void led_update_piano_war(led_update_function_data_t *data)
{
    led_update_piano_war_data_t *pattern_data = (led_update_piano_war_data_t *) data->pattern_data;

    //unlock all values
    for(int i = 0; i < LED_COUNT; i++)
    {
        if(pattern_data->locked[i]) pattern_data->locked[i] = 0;
    }

    for(int i = 0; i < LED_COUNT; i++)
    {
        if(pattern_data->occupied[i] && !pattern_data->locked[i])
        {
            if((i == 0 && pattern_data->direction[i] == -1) ||
               (i == LED_COUNT - 1 && pattern_data->direction[i] == 1))
            {
                pattern_data->size[i]--;

                if(pattern_data->size[i])
                {
                    pattern_data->colors[i] = random_near_color(pattern_data->colors[i],
                                                                   RAND_COLOR_THRESHOLD,
                                                                   RAND_COLOR_THRESHOLD,
                                                                   RAND_COLOR_THRESHOLD);

                    if(pattern_data->direction[i] == -1)
                    {
                        pattern_data->direction[i] = 1;
                    }
                    else if(pattern_data->direction[i] == 1)
                    {
                        pattern_data->direction[i] = -1;
                    }
                }
                else
                {
                    pattern_data->occupied[i] = 0;
                    pattern_data->colors[i] = 0;
                    pattern_data->direction[i] = 0;
                }
            }
            else
            {
                if(pattern_data->occupied[i + pattern_data->direction[i]])
                {
                    pattern_data->size[i]--;

                    if(pattern_data->size[i])
                    {
                        pattern_data->colors[i] = random_near_color(pattern_data->colors[i],
                                                                       RAND_COLOR_THRESHOLD,
                                                                       RAND_COLOR_THRESHOLD,
                                                                       RAND_COLOR_THRESHOLD);

                        if(pattern_data->direction[i] == -1)
                        {
                            pattern_data->direction[i] = 1;
                        }
                        else
                        {
                            pattern_data->direction[i] = -1;
                        }
                    }
                    else
                    {
                        pattern_data->occupied[i] = 0;
                        pattern_data->colors[i] = 0;
                        pattern_data->direction[i] = 0;
                    }
                }
                else
                {
                    pattern_data->occupied[i + pattern_data->direction[i]] = 1;
                    pattern_data->colors[i +
                                         pattern_data->direction[i]] = pattern_data->colors[i];
                    pattern_data->direction[i +
                                            pattern_data->direction[i]] = pattern_data->direction[i];
                    pattern_data->size[i +
                                       pattern_data->direction[i]] = pattern_data->size[i];
                    pattern_data->locked[i + pattern_data->direction[i]] = 1;

                    pattern_data->occupied[i] = 0;
                    pattern_data->colors[i] = 0;
                    pattern_data->direction[i] = 0;
                    pattern_data->size[i] = 0;
                }
            }
        }
    }

    int left_count = 0;
    int right_count = 0;

    while(pipe_size((pipe_generic_t *) data->data_pipe) > 0)
    {
        pipe_pop(data->data_pipe, data->buffer, 1);

        if(data->buffer[0] == 144)
        {
            pipe_pop(data->data_pipe, data->buffer, 2);

            if(data->buffer[1] > 0)
            {
                if(data->buffer[0] - 21 < 44)
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
        pattern_data->occupied[0] = 1;
        pattern_data->colors[0] = (unsigned int) (random() % 0xFFFFFF);
        pattern_data->direction[0] = 1;
        pattern_data->size[0] = left_count;
    }

    if(right_count > 0)
    {
        pattern_data->occupied[LED_COUNT - 1] = 1;
        pattern_data->colors[LED_COUNT - 1] = (unsigned int) (random() % 0xFFFFFF);
        pattern_data->direction[LED_COUNT - 1] = -1;
        pattern_data->size[LED_COUNT - 1] = right_count;
    }

    for(int i = 0; i < LED_COUNT; i++)
    {
        data->led_states[i] = pattern_data->colors[i];
    }
}
