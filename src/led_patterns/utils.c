#include "utils.h"
#include "piano.h"
#include "ambient.h"

#include <stdlib.h>

void set_data_for_pattern(led_update_function_data_t *data, Pattern pattern)
{
    //wipe LED states
    for(int i = 0; i < LED_COUNT; i++)
    {
        data->led_states[i] = 0;
    }

    data->buffer[0] = 0;
    data->buffer[1] = 0;

    //free up memory if we can
    if(data->pattern_data != NULL) free(data->pattern_data);

    //switch through patterns and set up the one that matches
    switch(pattern)
    {
        case NONE:
            data->current_pattern = NONE;
            data->last_pattern = NONE;
            data->current_update_function = NULL;
            data->pattern_data = NULL;
            break;

        case PIANO_NORMAL:
            data->current_pattern = PIANO_NORMAL;
            data->last_pattern = NONE;
            data->current_update_function = led_update_piano_normal;
            data->pattern_data = new_led_update_piano_normal_data_t();
            break;

        case PIANO_WAR:
            data->current_pattern = PIANO_WAR;
            data->last_pattern = NONE;
            data->current_update_function = led_update_piano_war;
            data->pattern_data = new_led_update_piano_war_data_t();
            break;

        case AMBIENT_NORMAL:
            data->current_pattern = AMBIENT_NORMAL;
            data->last_pattern = NONE;
            data->current_update_function = led_update_ambient_normal;
            data->pattern_data = new_led_update_ambient_normal_data_t();
            break;

        case AMBIENT_GRADIENT:
            data->current_pattern = AMBIENT_GRADIENT;
            data->last_pattern = NONE;
            data->current_update_function = led_update_ambient_gradient;
            data->pattern_data = new_led_update_ambient_gradient_data_t();
            break;
    }
}
