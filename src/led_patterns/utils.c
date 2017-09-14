#include "utils.h"
#include "piano.h"
#include "ambient.h"

#include <stdlib.h>

void set_data_for_pattern(led_update_function_data_t *data, PATTERN pattern)
{
    for(int i = 0; i < LED_COUNT; i++)
    {
        data->led_states[i] = 0;
    }

    if(data->pattern_data != NULL) free(data->pattern_data);

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
            data->pattern_data = new_led_update_piano_normal_data();
            break;

        case PIANO_WAR:
            data->current_pattern = PIANO_WAR;
            data->last_pattern = NONE;
            data->current_update_function = led_update_piano_war;
            data->pattern_data = new_led_update_piano_war_data();
            break;

        case AMBIENT_NORMAL:
            data->current_pattern = AMBIENT_NORMAL;
            data->last_pattern = NONE;
            data->current_update_function = led_update_ambient_normal;
            data->pattern_data = new_led_update_ambient_normal_data();
            break;
    }
}
