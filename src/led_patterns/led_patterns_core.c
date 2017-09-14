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
        data->pattern_data = data->new_current_update_function_data();
    }

    if(data->current_pattern != data->last_pattern)
    {
        if(data->pattern_data != NULL) free(data->pattern_data);
        data->pattern_data = data->new_current_update_function_data();
    }

    data->current_update_function(data);

    data->last_pattern = data->current_pattern;
}