#include "ambient.h"
#include "color_utils.h"

#include <stdlib.h>

/**
 * Internal struct to hold all working data for the led_update_ambient_normal function
 */
typedef struct led_update_piano_ambient_data
{
    unsigned int last_color;      /**< The last color used by the led_update_piano_normal function*/
} led_update_piano_ambient_data_t;

void *new_led_update_ambient_normal_data_t()
{
    led_update_piano_ambient_data_t *ret = malloc(sizeof(led_update_piano_ambient_data_t));

    //set everything to 0
    ret->last_color = 0;
    
    return ret;
}

void led_update_ambient_normal(led_update_function_data_t *data)
{
    unsigned int color = ((led_update_piano_ambient_data_t *) data->pattern_data)->last_color;

    if(color == 0)
    {
        //generate a new color
        color = (unsigned int) random();
    }
    else
    {
        //get a close color
        color = random_near_color(color,
                                  RAND_COLOR_THRESHOLD,
                                  RAND_COLOR_THRESHOLD,
                                  RAND_COLOR_THRESHOLD);
    }

    for(int i = 0; i < LED_COUNT; i++)
    {
        //introduce some randomness
        unsigned int adj_color = random_near_color(color,
                                                   RAND_COLOR_THRESHOLD * 3,
                                                   RAND_COLOR_THRESHOLD * 3,
                                                   RAND_COLOR_THRESHOLD * 3);

        //copy color to led_states[]
        data->led_states[i] = adj_color;
    }
}
