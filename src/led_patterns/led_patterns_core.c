#include "led_patterns_core.h"

#include <stdio.h>
#include <stdlib.h>

led_update_function_data_t *new_led_update_function_data_t()
{
    led_update_function_data_t *ret = malloc(sizeof(led_update_function_data_t));

    ret->current_pattern = NONE;
    ret->last_pattern = NONE;
    ret->pattern_data = NULL;

    ret->consumer = NULL;

    return ret;
}

void run_led_update_function(led_update_function_data_t *data)
{
    //reset buffer
    data->buffer[0] = 0;
    data->buffer[1] = 0;

    if(data->last_pattern == NONE)
    {
        data->last_pattern = data->current_pattern;

        if(data->current_pattern == PIANO_NORMAL)
        {
            data->pattern_data = new_led_update_piano_normal_data();
        }

        if(data->current_pattern == PIANO_WAR)
        {
            data->pattern_data = new_led_update_piano_war_data();
        }
    }

    if(data->current_pattern != data->last_pattern)
    {
        if(data->pattern_data != NULL) free(data->pattern_data);

        if(data->current_pattern == PIANO_NORMAL)
        {
            data->pattern_data = new_led_update_piano_normal_data();
        }

        if(data->current_pattern == PIANO_WAR)
        {
            data->pattern_data = new_led_update_piano_war_data();
        }
    }

    switch(data->current_pattern)
    {
        case PIANO_NORMAL:
            led_update_piano_normal(data);
            break;

        case PIANO_WAR:
            led_update_piano_war(data);
            break;

        case NONE:
            //something weird happened, we should probably just crash
            exit(24601);
    }

    data->last_pattern = data->current_pattern;
}