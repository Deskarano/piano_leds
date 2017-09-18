#include "utils.h"

#include <stdlib.h>

led_update_function_data_t *new_led_update_function_data_t()
{
    led_update_function_data_t *ret = malloc(sizeof(led_update_function_data_t));

    //set up all values to defaults
    ret->current_pattern = NONE;
    ret->last_pattern = NONE;
    ret->current_update_function = NULL;
    ret->pattern_data = NULL;
    ret->led_states = malloc(LED_COUNT * sizeof(unsigned int));
    ret->buffer = malloc(2 * sizeof(unsigned char));
    ret->data_pipe = NULL;

    return ret;
}

void free_led_update_function_data_t(led_update_function_data_t *data)
{
    set_data_for_pattern(data, NONE);
    free(data->led_states);
    free(data->buffer);
    free(data);
}

void run_led_update_function(led_update_function_data_t *data)
{
    //reset buffer
    data->buffer[0] = 0;
    data->buffer[1] = 0;

    //set up data for first use
    if(data->last_pattern == NONE)
    {
        data->last_pattern = data->current_pattern;
        set_data_for_pattern(data, data->current_pattern);
    }

    //set up data if the LED pattern changed
    if(data->current_pattern != data->last_pattern)
    {
        set_data_for_pattern(data, data->current_pattern);
    }

    //run the update function and update last_pattern
    data->current_update_function(data);
    data->last_pattern = data->current_pattern;
}