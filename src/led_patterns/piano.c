#include "piano.h"
#include "color_utils.h"
#include "../pipe/pipe.h"

#include <stdlib.h>

/**
 * Internal struct to hold all working data for the led_update_piano_normal function
 */
typedef struct led_update_piano_normal_data
{
    int key_pressed[LED_COUNT];         /**< Array storing whether a given piano key is pressed or not */
    int key_sustain[LED_COUNT];         /**< Array storing whether to sustain a given key or not */

    int sustain;                        /**< Only ever 1 or 0. Indicates if the sustain pedal is pressed or not */

    unsigned int last_color;            /**< The last color used by the led_update_piano_normal function*/
} led_update_piano_normal_data_t;

/**
 * Internal struct to hold all working data for the led_update_piano_war function
 */
typedef struct led_update_piano_war_data
{
    int occupied[LED_COUNT];            /**< Array storing whether a given LED is occupied */
    unsigned int colors[LED_COUNT];     /**< Array storing current colors of all LEDs */
    int direction[LED_COUNT];           /**< Array storing which direction a given LED is "traveling" in */
    int size[LED_COUNT];                /**< Array storing the size of a given LED */
    int locked[LED_COUNT];              /**< Array controlling locking and unlocking of LEDs */
} led_update_piano_war_data_t;

void *new_led_update_piano_normal_data_t()
{
    led_update_piano_normal_data_t *ret = malloc(sizeof(led_update_piano_normal_data_t));

    //initialize everything to 0
    for(int i = 0; i < LED_COUNT; i++)
    {
        ret->key_pressed[i] = 0;
        ret->key_sustain[i] = 0;
    }

    ret->last_color = 0;
    ret->sustain = 0;

    return ret;
}

void *new_led_update_piano_war_data_t()
{
    led_update_piano_war_data_t *ret = malloc(sizeof(led_update_piano_war_data_t));

    //initialize everything to 0
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
    //copy pattern_data into a local variable for less typing
    led_update_piano_normal_data_t *pattern_data = (led_update_piano_normal_data_t *) data->pattern_data;

    if(pipe_size((pipe_generic_t *) data->data_pipe) > 0)
    {
        //pick a color to use for all keys currently in the pipe
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
                //pop contained a new key, analyze its number and velocity
                pipe_pop(data->data_pipe, data->buffer, 2);

                if(data->buffer[1] > 0)
                {
                    //keys that are hit harder create brighter LEDs
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
            else if(data->buffer[0] == 176)
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

    //if not sustaining, remove any keys from key_sustain[] that are not key_pressed[]
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

    //fade out sustained LEDs slower than non-sustained LEDs
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
    //copy pattern_data into a local variable for less typing
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
                //corner impact cases
                pattern_data->size[i]--;

                if(pattern_data->size[i])
                {
                    //LED is alive; change color
                    pattern_data->colors[i] = random_near_color(pattern_data->colors[i],
                                                                RAND_COLOR_THRESHOLD,
                                                                RAND_COLOR_THRESHOLD,
                                                                RAND_COLOR_THRESHOLD);

                    //switch direction
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
                    //LED is dead; erase it
                    pattern_data->occupied[i] = 0;
                    pattern_data->colors[i] = 0;
                    pattern_data->direction[i] = 0;
                }
            }
            else
            {
                if(pattern_data->occupied[i + pattern_data->direction[i]])
                {
                    //LED collision case
                    pattern_data->size[i]--;

                    if(pattern_data->size[i])
                    {
                        //LED is alive; change color
                        pattern_data->colors[i] = random_near_color(pattern_data->colors[i],
                                                                    RAND_COLOR_THRESHOLD,
                                                                    RAND_COLOR_THRESHOLD,
                                                                    RAND_COLOR_THRESHOLD);

                        //switch direction
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
                        //LED is dead; erase it
                        pattern_data->occupied[i] = 0;
                        pattern_data->colors[i] = 0;
                        pattern_data->direction[i] = 0;
                    }
                }
                else
                {
                    //move case: shift everything by one in the direction of travel
                    pattern_data->occupied[i + pattern_data->direction[i]] = 1;
                    pattern_data->colors[i + pattern_data->direction[i]] = pattern_data->colors[i];
                    pattern_data->direction[i + pattern_data->direction[i]] = pattern_data->direction[i];
                    pattern_data->size[i + pattern_data->direction[i]] = pattern_data->size[i];
                    pattern_data->locked[i + pattern_data->direction[i]] = 1;

                    pattern_data->occupied[i] = 0;
                    pattern_data->colors[i] = 0;
                    pattern_data->direction[i] = 0;
                    pattern_data->size[i] = 0;
                }
            }
        }
    }

    //add new LEDs
    int left_count = 0;
    int right_count = 0;

    while(pipe_size((pipe_generic_t *) data->data_pipe) > 0)
    {
        pipe_pop(data->data_pipe, data->buffer, 1);

        if(data->buffer[0] == 144)
        {
            //pipe contained a new key
            pipe_pop(data->data_pipe, data->buffer, 2);

            if(data->buffer[1] > 0)
            {
                //LEDs from left keys come from left, right keys come from right
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

    //create new LEDs
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

    //copy data into led_states[]
    for(int i = 0; i < LED_COUNT; i++)
    {
        data->led_states[i] = pattern_data->colors[i];
    }
}
