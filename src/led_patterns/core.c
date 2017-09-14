#include "utils.h"

#include <stdlib.h>

led_update_function_data_t *new_led_update_function_data_t()
{
    led_update_function_data_t *ret = malloc(sizeof(led_update_function_data_t));

    ret->current_pattern = NONE;
    ret->last_pattern = NONE;
    ret->pattern_data = NULL;

    ret->data_pipe = NULL;

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
        set_data_for_pattern(data, data->current_pattern);
    }

    if(data->current_pattern != data->last_pattern)
    {
        set_data_for_pattern(data, data->current_pattern);
    }

    data->current_update_function(data);
    data->last_pattern = data->current_pattern;
}