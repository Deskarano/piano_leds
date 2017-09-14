#include "led_patterns_ambient.h"
#include "color_utils.h"

#include <stdlib.h>

typedef struct led_update_piano_ambient_data
{
    unsigned int last_color;
} led_update_piano_ambient_data_t;

void *new_led_update_ambient_normal_data()
{

}


void led_update_ambient_normal(led_update_function_data_t *data)
{
    unsigned int color = ((led_update_piano_ambient_data_t *) data->pattern_data)->last_color;

    if(color == 0)
    {
        color = (unsigned int) random();
    }
    else
    {
        color = random_near_color(color,
                                  RAND_COLOR_THRESHOLD,
                                  RAND_COLOR_THRESHOLD,
                                  RAND_COLOR_THRESHOLD);
    }

    for(int i = 0; i < LED_COUNT; i++)
    {
        unsigned int adj_color = random_near_color(color,
                                                   RAND_COLOR_THRESHOLD * 3,
                                                   RAND_COLOR_THRESHOLD * 3,
                                                   RAND_COLOR_THRESHOLD * 3);

        data->led_states[i] = adj_color;
    }
}
