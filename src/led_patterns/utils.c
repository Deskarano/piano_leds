#include <stddef.h>

#include "utils.h"
#include "piano.h"
#include "ambient.h"

void set_data_for_pattern(led_update_function_data_t *data, PATTERN pattern)
{
    switch(pattern)
    {
        case NONE:
            data->current_pattern = NULL;
            data->last_pattern = NULL;
            data->current_update_function = NULL;
            data->new_current_update_function_data = NULL;
            break;

        case PIANO_NORMAL:
            data->current_pattern = PIANO_NORMAL;
            data->last_pattern = NULL;
            data->current_update_function = led_update_piano_normal;
            data->new_current_update_function_data = new_led_update_piano_normal_data;
            break;

        case PIANO_WAR:
            data->current_pattern = PIANO_WAR;
            data->last_pattern = NULL;
            data->current_update_function = led_update_piano_war;
            data->new_current_update_function_data = new_led_update_piano_war_data;
            break;

        case AMBIENT_NORMAL:
            data->current_pattern = AMBIENT_NORMAL;
            data->last_pattern = NULL;
            data->current_update_function = led_update_ambient_normal;
            data->new_current_update_function_data = new_led_update_ambient_normal_data;
            break;
    }
}
